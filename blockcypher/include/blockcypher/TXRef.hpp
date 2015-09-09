/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <common/TransactionId.hpp>

#ifndef BLOCKCYPHER_TXREF_HPP
#define BLOCKCYPHER_TXREF_HPP

namespace BlockCypher {

    /**
     * TXRef (http://dev.blockcypher.com/#txref)
     * =========================================
     * A TXRef object represents summarized data about
     * a transaction input or output. Typically found in an array
     * within an Address object, which
     * is usually returned from the
     * standard Address Endpoint.
     */
    struct TXRef {

        // Height of the block that contains this transaction.
        // If this is an unconfirmed transaction, it will equal -1.
        int _block_height;

        // The hash of the transaction containing this input or output.
        // While reasonably unique, using hashes as identifiers may be unsafe.
        Coin::TransactionId _tx_hash;

        // Index of the input in the transaction. It’s a negative number for an output.
        int _tx_input_n;

        // Index of the output in the transaction. It’s a negative number for an input.
        int _tx_output_n;

        // The value transfered by this input/output in satoshis exchanged in this transaction.
        quint64 _value;

        // The likelihood that this transaction will make it to the next block;
        // reflects the preference level miners have to include this transaction.
        // Can be high, medium or low.

        enum class Preference {
            high,
            medium,
            low
        };

        Preference preference;

        // True if this output was spent; false otherwise.
        bool spent;

        // True if this is an attempted double spend; false otherwise.
        bool doble_spend;

        // Number of subsequent blocks,
        // including the block the transaction is in.
        // Unconfirmed transactions have 0 confirmations.
        quint32 _confirmations;
    };

}

#endif // BLOCKCYPHER_TXREF_HPP

