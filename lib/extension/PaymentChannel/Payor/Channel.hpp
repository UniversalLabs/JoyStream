#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutputPoint.hpp"

/**
 * Represents a single channel.
 */
class Channel {

public:

    /**
     * @brief Enumeration of possible channel states.
     */
    enum class State {
        unassigned,
        assigned,
        refund_signed
    };

    /**
     * @brief The configurations of a channel set by the payor
     *
     * IS THIS NEEDED ???
     */
    class PayorConfiguration {

    public:

        // Constructor
        PayorConfiguration(quint64 funds,
                           const KeyPair & contractKeyPair,
                           const KeyPair & finalKeyPair,
                           quint32 refundLockTime);

        // Getters and setters
        quint64 funds() const;
        void setFunds(const quint64 &funds);

        KeyPair contractKeyPair() const;
        void setContractKeyPair(const KeyPair &contractKeyPair);

        KeyPair finalKeyPair() const;
        void setFinalKeyPair(const KeyPair &finalKeyPair);

        quint32 getRefundLockTime() const;
        void setRefundLockTime(const quint32 &value);

    private:

        // Funds allocated to output
        quint64 _funds;

        // Controls payour output of multisig
        KeyPair _contractKeyPair;

        // Controls final payment to payor
        KeyPair _finalKeyPair;

        // Refund lock time
        quint32 _refundLockTime;
    };

    /**
     * @brief The configurations of a channel set by the payee
     */
    class PayeeConfiguration {

    public:

        // Constructor
        PayeeConfiguration(quint64 price, const PublicKey &contractPk, const PublicKey &finalPk, quint64 paymentFee);

        // Getters and setters
        quint64 price() const;
        void setPrice(const quint64 &price);

        PublicKey contractPk() const;
        void setContractPk(const PublicKey &contractPk);

        PublicKey finalPk() const;
        void setFinalPk(const PublicKey &finalPk);

        quint64 paymentFee() const;
        void setPaymentFee(const quint64 &paymentFee);

    private:

        // Size of single payment
        quint64 _price;

        // Controls payee output of multisig, received in joinin_contract.pk
        PublicKey _contractPk;

        // Controls payee payments, received in sign_refund.pk
        PublicKey _finalPk;

        // Fee used in payment transaction
        quint64 _paymentFee;
    };

    // Default/Copy constructor and assignemtn operator needed to put in container.
    Channel();
    Channel(const Channel& slot);
    Channel & operator=(const Channel& rhs);

    // Set all fields, e.g. loading from file
    Channel(quint32 index,
         const State &state,
         quint64 price,
         quint64 numberOfPaymentsMade,
         quint64 funds,
         const KeyPair &payorContractKeyPair,
         const PublicKey &payorFinalPk,
         const PublicKey &payeeContractPk,
         const PublicKey &payeeFinalPk,
         const Signature &refund,
         quint64 refundFee,
         quint64 paymentFee,
         quint32 refundLockTime);

    /**
     * Payment channel operations
     */

    // Refund transaction for slot
    //Refund refund(const Hash &contractHash) const;

    // Payment transaction for slot, based on current _numberOfPaymentsMade value
    //Payment payment(const Hash &contractHash) const;

    // Compute payor refund signature
    void computePayorRefundSignature(const Hash &contractHash) const;

    // Payment signature
    Signature paymentSignature(const Hash &contractHash) const;

    // Registers that a payment was made
    void paymentMade();

    QJsonObject json() const;

    /**
     * Getters and setters
     */

    quint32 index() const;
    void setIndex(const quint32 &index);

    State state() const;
    void setState(const State &state);

    quint64 price() const;
    void setPrice(const quint64 &price);

    quint64 numberOfPaymentsMade() const;
    void setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade);

    quint64 funds() const;
    void setFunds(const quint64 &funds);

    KeyPair payorContractKeyPair() const;
    void setPayorContractKeyPair(const KeyPair &keyPair);

    PublicKey payorFinalPk() const;
    void setPayorFinalPk(const PublicKey &pk);

    PublicKey payeeContractPk() const;
    void setPayeeContractPk(const PublicKey &pk);

    KeyPair payorFinalKeyPair() const;
    void setPayorFinalKeyPair(const KeyPair &payorFinalKeyPair);

    quint64 paymentFee() const;
    void setPaymentFee(const quint64 &paymentFee);

    PublicKey payeeFinalPk() const;
    void setPayeeFinalPk(const PublicKey &payeeFinalPk);

    quint32 refundLockTime() const;
    void setRefundLockTime(const quint32 &refundLockTime);

    Signature payorRefundSignature() const;
    void setPayorRefundSignature(const Signature &payorRefundSignature);

    Signature payeeRefundSignature() const;
    void setPayeeRefundSignature(const Signature &payeeRefundSignature);

private:

    // Index in contract
    quint32 _index;

    // Slot state
    State _state;

    // Size of single payment
    quint64 _price;

    // Number of payments made
    quint64 _numberOfPaymentsMade;

    // Funds allocated to output
    quint64 _funds;

    // Controls payour output of multisig
    KeyPair _payorContractKeyPair;

    // Controls final payment to payor
    KeyPair _payorFinalKeyPair;

    // Controls payee output of multisig, received in joinin_contract.pk
    PublicKey _payeeContractPk;

    // Controls payee payments, received in sign_refund.pk
    PublicKey _payeeFinalPk;

    // Controls refund for payor
    Signature _payorRefundSignature;

    // Controls refund for payee
    Signature _payeeRefundSignature;

    // Fee used in refund transaction, is unlikely to vary across slots,
    quint64 _refundFee;

    // Fee used in payment transaction
    quint64 _paymentFee;

    // Lock time of refund, received in
    quint32 _refundLockTime;
};

#endif // CHANNEL_HPP
