#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "PaymentChannel/Payor.hpp"
#include "BuyerPeerPlugin.hpp"

//#include <QTime>

#include <queue>          // std::priority_queue

class Wallet;

/**
 * @brief Torrent plugin for buyer mode.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    /**
     * @brief Plugin state
     */
    enum class State {

        // _payor has reached, or passed, Payor:State::paying status
        waiting_for_payor_to_be_ready,

        // Requesting and downloading pieces
        downloading_pieces,

        // Have full torrent
        done
    };

    /**
     * @brief Represents piece in torrent
     */
    class Piece {

    public:

        enum class State {

            // We do not have piece, and it has given assignment status
            unassigned,
            assigned,

            // We do have piece
            fully_downloaded_and_valid,
        };

        // Default constructor
        Piece();

        // Constructors based on members
        Piece(int index, int length, int numberOfBlocks, State state, BuyerPeerPlugin * peerPlugin);

        // Getters and setters
        int index() const;
        void setIndex(int index);

        int length() const;
        void setLength(int length);

        int numberOfBlocks() const;
        void setNumberOfBlocks(int numberOfBlocks);

        State state() const;
        void setState(const State &state);

        BuyerPeerPlugin * peerPlugin() const;
        void setPeerPlugin(BuyerPeerPlugin * peerPlugin);

    private:

        // Index of piece
        int _index;

        // Byte length of piece (should be the same for all but last piece)
        int _length;

        // Number of blocks in piece
        int _numberOfBlocks;

        // Piece state
        State _state;

        // Peer plugin assigned to this piece
        BuyerPeerPlugin * _peerPlugin;
    };

    /**
     * @brief Plugin status, that is a snapshot
     * of important information.
     */
    class Status {

    public:

        Status(State state,
                const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                const Payor::Status & payor);

        // Getters and Setters
        State state() const;
        void setState(State state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peers() const;
        void setPeers(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers);

        Payor::Status payor() const;
        void setPayor(const Payor::Status & payor);

    private:

        // State of plugin
        State _state;

        // Status of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> _peers;

        // Status of the payor
        Payor::Status _payor;
    };

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

        // Constructor from copy
        Configuration(const Configuration & c);

        // Constructor from members
        Configuration(bool enableBanningSets,
                      State state,
                      //const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> & peerConfigurations,
                      const Payor::Configuration & payorConfiguration);

        /**
        // Constructor for a fresh plugin.
        Configuration(QVector<quint64> funds,
                      quint64 changeValue,
                      const OutPoint & fundingOutput,
                      const KeyPair & fundingOutputKeyPair,
                      quint64 maxPrice,
                      quint32 maxLock);
        */

        // Constructor from dictionary
        Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        /**
         * Write configuration into dictionary
         * ===============================================================
         *
         * Buyer torrent plugin configuration as it persists across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * IMPLEMENT LATER
         *
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Getters and setters
        virtual PluginMode pluginMode() const;

        State state() const;
        void setState(const State & state);

        /*
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> peerConfigurations() const;
        void setPeerConfigurations(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> &peerConfigurations);
        */

        Payor::Configuration payorConfiguration() const;
        void setPayorConfiguration(const Payor::Configuration & payorConfiguration);

    private:

        // What stage is plugin
        State _state;

        // DEPRECATED FOR NOW
        // Configuration of peers
        //QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> _peerConfigurations;

        // Configuration of payor
        Payor::Configuration _payorConfiguration;
    };

    // Constructor from members
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                       Wallet * wallet,
                       const Configuration & configuration,
                       QLoggingCategory & category);

    /**
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                       quint32 _numSellers,
                       QLoggingCategory & category);
    */

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool on_resume();
    virtual bool on_pause();
    virtual void on_files_checked();
    virtual void on_state(int s);
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);

    // Get peer_plugin if present, otherwise NULL pointer is wrapped
    //virtual boost::weak_ptr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    // Checks if seller can be invited on given terms
    bool inviteSeller(quint32 minPrice, quint32 minLock) const;

    // Attempts to add seller to contract
    bool sellerWantsToJoinContract(BuyerPeerPlugin * peer, quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime);

    // Verifies signature, and also broadcasts contract if full set of signatures has been aquired
    bool sellerProvidedRefundSignature(BuyerPeerPlugin * peer, const Signature & refundSignature);

    // Find peers which presently have not been assigned a piece, and assign piece.
    bool assignPieceToPeerPlugin(BuyerPeerPlugin * peer);

    // Starting after startIndex, and finds the first piece which has not been downloaded and not assigned.
    // If no piece is found greater than startIndex, it loops around from piece 0
    // to see if there are any hits there. If this fails, it throws and exception
    // to signify that all pieces have been assigned
    int getNextUnassignedPiece(int startIndex) const;

    // Marks corresponding piece as downloaded
    void pieceDownloaded(int index);

    // Increments payment counter in payment channel, and generates signature
    Signature makePaymentAndGetPaymentSignature(BuyerPeerPlugin * peerPlugin);

    // Generate plugin status
    Status status() const;

    // Getters and setters
    virtual PluginMode pluginMode() const;

    State state() const;
    void setState(const State & state);

    quint64 maxPrice() const;
    quint32 maxLock() const;

    // Allows
    //const Payor & payor() const;

    int blockSize() const;
    void setBlockSize(int blockSize);

private:

    // What stage is plugin
    State _state;

    // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
    // the type of weak_ptr libtrrrent requires, hence might as well put it
    // in this type, rather than corresponding subclass of TorrentPlugin.
    QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> > _peers;

    // Maps given position in payor to given end point
    /**
     * LATER, ADD SOME SORT OF ELEMENT FIELD TO THE CHANNELS OF THE PAYOR,
     * SO THAT THERE IS TIGHTER ASSOCIATION BETWEEN CHANNELS AND
     * Lets just use sloppy pointers for now!!!!!!!!
     */
    QVector<BuyerPeerPlugin *> _slotToPluginMapping;

    // Wallet
    Wallet * _wallet;

    // Payment channel
    Payor _payor;

    // Time since plugin was created, is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    /**
     * Piece management
     */

    // Pieces in torrent file
    QVector<Piece> _pieces;

    // The blocksize in the torrent
    int _blockSize;

    // The number of pieces which have not been downloaded and not been assigned to a peer
    quint32 _numberOfUnassignedPieces;

    // Set of peer plugins which have not been assigned a piece.
    // the tick() callback routinely attempts to assign a piece
    // to a peer plugin in this set.
    QSet<BuyerPeerPlugin *> _peerPluginsWithoutPieceAssignment;

    // Keeps track of lower bound for piece indexes which may be assigned.
    // Is updated when full pieces are downloaded contigously, and
    // is used with getNextUnassignedPiece() to find next piece to assign.
    //
    // Is required to ensure in order downloading from correct position in file, e.g.,
    // if user has seeked to this position recently.
    int _assignmentLowerBound;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
