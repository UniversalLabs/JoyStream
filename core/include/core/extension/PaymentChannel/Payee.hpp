/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYEE_HPP
#define PAYEE_HPP

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class Transaction;
}

/**
 * Manages the payee side of a 1-to-N payment channel using design in CBEP.
 * https://github.com/bedeho/CBEP
 */
class Payee {

public:

    /**
     * @brief The payee state
     */
    enum class State {

        // Information, provided by payee, required for receiving payments has not been set (using registerPayeeInformation)
        // WHY DO WE EVEN NEED THIS, WE CONTROL THIS INFORMATION ANYWAY
        // waiting_for_payee_information,

        // Information, provided by payor, required for receiving payments has not been set (using registerPayorInformation)
        waiting_for_payor_information,

        // All information required to sign refunds and validate payments has been provided
        has_all_information_required,

        /**
        //
        final_payment_spent,

        //
        refund_spent,
        */
    };

    /**
     * @brief The status of payee
     */
    class Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(State state, quint64 numberOfPaymentsMade, quint32 lockTime, quint64 price, const Coin::typesafeOutPoint & contractOutPoint, quint64 funds);

        // Getters and setters
        State state() const;
        void setState(State state);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        quint32 lockTime() const;
        void setLockTime(quint32 lockTime);

        quint64 price() const;
        void setPrice(quint64 price);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint);

        quint64 funds() const;
        void setFunds(quint64 funds);

    private:

        // Payee state
        State _state;

        // The number of payments which have been successfully made
        quint64 _numberOfPaymentsMade;

        // Payment channel lock time
        quint32 _lockTime;

        // Price increment per payment
        quint64 _price;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Amount (#satoshies) assigned to contract output
        quint64 _funds;
    };

    /**
     * @brief Persistant state of payee
     */
    class Configuration {

    public:

        // Default constructor
        Configuration();

        // Constructor from members
        Configuration(State state,
                      quint64 numberOfPaymentsMade,
                      const Coin::Signature & lastValidPayorPaymentSignature,
                      quint32 lockTime,
                      quint64 price,
                      quint32 maximumNumberOfSellers,
                      const Coin::KeyPair & payeeContractKeys,
                      const Coin::KeyPair & payeePaymentKeys,
                      const Coin::typesafeOutPoint & contractOutPoint,
                      const Coin::PublicKey & payorContractPk,
                      const Coin::PublicKey & payorFinalPk,
                      quint64 funds);

        // Getters and setters
        State state() const;
        void setState(State state);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        Coin::Signature lastValidPayorPaymentSignature() const;
        void setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature);

        quint32 lockTime() const;
        void setLockTime(quint32 lockTime);

        quint64 price() const;
        void setPrice(quint64 price);

        quint32 maximumNumberOfSellers() const;
        void setMaximumNumberOfSellers(quint32 maximumNumberOfSellers);

        Coin::KeyPair payeeContractKeys() const;
        void setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys);

        Coin::KeyPair payeePaymentKeys() const;
        void setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint);

        Coin::PublicKey payorContractPk() const;
        void setPayorContractPk(const Coin::PublicKey & payorContractPk);

        Coin::PublicKey payorFinalPk() const;
        void setPayorFinalPk(const Coin::PublicKey & payorFinalPk);

        quint64 funds() const;
        void setFunds(quint64 funds);

    private:

        // Payee state
        State _state;

        // The number of payments which have been successfully made
        quint64 _numberOfPaymentsMade;

        // The last valid payment signature received, corresponds to _numberOfPaymentsMade
        Coin::Signature _lastValidPayorPaymentSignature;

        // Payment channel lock time
        quint32 _lockTime;

        // Price increment per payment
        quint64 _price;

        // Maximum number of non change outputs of payment channel contract
        quint32 _maximumNumberOfSellers;

        // Controls payee portion of contract output
        Coin::KeyPair _payeeContractKeys;

        // Controls payee output in payment _lastValidPaymentSignature
        Coin::KeyPair _payeePaymentKeys;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Payor key in contract output
        Coin::PublicKey _payorContractPk;

        // Payor key in output in refund and payment
        Coin::PublicKey _payorFinalPk;

        // Amount (#satoshies) assigned to contract output
        quint64 _funds;

    };

    // Default constructor
    Payee();

    // Constructor based on members
    Payee(State state,
            quint64 numberOfPaymentsMade,
            const Coin::Signature & lastValidPayorPaymentSignature,
            quint32 lockTime,
            quint64 price,
            quint32 maximumNumberOfSellers,
            const Coin::KeyPair & payeeContractKeys,
            const Coin::KeyPair & payeePaymentKeys,
            const Coin::typesafeOutPoint & contractOutPoint,
            const Coin::PublicKey & payorContractPk,
            const Coin::PublicKey & payorFinalPk,
            quint64 funds);

    // Constructor based on configuration
    Payee(const Payee::Configuration & configuration);

    /**
    // When payee configurations are chosen
    void registerPayeeInformation(quint32 lockTime, quint32 price, quint32 maximumNumberOfSellers, const KeyPair & payeeContractKeys, const KeyPair & payeePaymentKeys);
    */

    // When contract information is known, as advertised in
    void registerPayorInformation(const Coin::typesafeOutPoint & contractOutPoint, const Coin::PublicKey & payorContractPk, const Coin::PublicKey & payorFinalPk, quint64 funds);

    // Creates refund signature
    // ==================================================
    Coin::Signature generateRefundSignature() const;

    // Attempts to register payment if signature is valid
    // ==================================================
    // A valid signature will lead to an increase of _numberOfPaymentsMade,
    // and storing signature in _lastValidPayorPaymentSignature
    bool registerPayment(const Coin::Signature & payorPaymentSignature);

    // Attempts to check validity of given payment signature with (_numberOfPaymentsMade + 1)
    bool checkNextPaymentSignature(const Coin::Signature & payorPaymentSignature) const;

    // Attempts to validate the contract transaction
    bool validateContractTrasaction(const Coin::Transaction & transaction) const;

    // Generates transaction for last payment
    Coin::Transaction lastPaymentTransaction() const;

    /**
     * Routines below check contract validity in various ways
    // Returns the rate at which peers have output point
    // of contract in mempool or chain.
    float outputPointVisible() const;

    // Checks contract validitity
    // ==========================
    // 1) There is tx with _contractOutputPoint in longest chain
    // 2) tx has contract output point
    // 3) scriptSig of tx is controlled by given keys and has the correct quanitity of funds
    // 4) channel has to correct number of participants.
    bool isContractValid() const;
    */

    // Returns status of payee
    Status status() const;

    // Getters and setters
    State state() const;
    void setState(State state);

    quint64 numberOfPaymentsMade() const;
    void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

    Coin::Signature lastValidPayorPaymentSignature() const;
    void setLastValidPayorPaymentSignature(const Coin::Signature & lastValidPayorPaymentSignature);

    quint32 lockTime() const;
    void setLockTime(quint32 lockTime);

    quint64 price() const;
    void setPrice(quint64 price);

    quint32 maximumNumberOfSellers() const;
    void setMaximumNumberOfSellers(quint32 maximumNumberOfSellers);

    Coin::KeyPair payeeContractKeys() const;
    void setPayeeContractKeys(const Coin::KeyPair & payeeContractKeys);

    Coin::KeyPair payeePaymentKeys() const;
    void setPayeePaymentKeys(const Coin::KeyPair & payeePaymentKeys);

    Coin::typesafeOutPoint contractOutPoint() const;
    void setContractOutPoint(const Coin::typesafeOutPoint & contractOutPoint);

    Coin::PublicKey payorContractPk() const;
    void setPayorContractPk(const Coin::PublicKey & payorContractPk);

    Coin::PublicKey payorFinalPk() const;
    void setPayorFinalPk(const Coin::PublicKey & payorFinalPk);

    quint64 funds() const;
    void setFunds(quint64 funds);

private:

    // Payee state
    State _state;

    // The number of payments which have been successfully made
    quint64 _numberOfPaymentsMade;

    // The last valid payment signature received, corresponds to _numberOfPaymentsMade
    Coin::Signature _lastValidPayorPaymentSignature;

    // Payment channel lock time
    quint32 _lockTime;

    // Price increment per payment
    quint64 _price;

    // Maximum numer of sellers in contract
    quint32 _maximumNumberOfSellers;

    // Controls payee portion of contract output
    Coin::KeyPair _payeeContractKeys;

    // Controls payee output in payment _lastValidPaymentSignature
    Coin::KeyPair _payeePaymentKeys;

    // Contract outpoint from which payments originate
    Coin::typesafeOutPoint _contractOutPoint;

    // Payor key in contract output
    Coin::PublicKey _payorContractPk;

    // Payor key in output in refund and payment
    Coin::PublicKey _payorFinalPk;

    // Amount (#satoshies) assigned to contract output
    quint64 _funds;

};

#endif // PAYEE_HPP
