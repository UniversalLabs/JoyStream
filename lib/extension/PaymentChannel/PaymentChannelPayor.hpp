#ifndef PAYMENT_CHANNEL_PAYOR_HPP
#define PAYMENT_CHANNEL_PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutputPoint.hpp"

class RefundTransaction;
class PaymentTransaction;
class ContractTransaction;

/**
 * 1-to-N payment channel from payor perspective, using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class PaymentChannelPayor
{
public:

    /**
     * @brief State of a payor.
     */
    enum class State {

        // Getting keys and refund signature
        constructing_channel,

        // Full set of signatures acheived
        all_refunds_collected,

        //
        contract_broadcasted,


        all_refunds_uspendable
    };

    /**
     * Represents payment channel participant as seen by a payor.
     */
    class Slot {

    public:

        /**
         * @brief Enumeration of possible slot states.
         */
        enum class State {
            unassigned_payee,
            received_payee_information,
            refund_signed
        };

        // Default/Copy constructor and assignemtn operator needed to put in container.
        Slot();
        Slot(const Slot& slot);
        Slot & operator=(const Slot& rhs);

        // Set all fields, e.g. loading from file
        Slot(const State & state,
             quint64 numberOfPaymentsMade,
             quint64 funds,
             quint64 priceIncrement,
             quint32 index,
             const KeyPair& payorKeyPair,
             const PublicKey & payeeContractPk,
             const PublicKey & payeeFinalPk,
             const Signature & refund);

        // Start before contract has been constructed
        Slot(quint64 funds,
             quint64 priceIncrement,
             quint32 index,
             const KeyPair& payorKeyPair);

        /**
         * Payment channel operations
         */

        // Refund transaction for slot
        RefundTransaction refundTransaction() const;

        // Payment transaction for slot, based on current _numberOfPaymentsMade value
        PaymentTransaction paymentTransaction() const;

        // Checks if signature is valid for refund in index'th slot
        bool isRefundValid(Signature payeeSignature);

        // Provides payment signature
        Signature nextPaymentSignature() const;

        // Registers that a payment was made
        void paymentMade();

        // Checks if slot payment has been published
        bool isRefundDoubleSpent() const;

        /**
         * Getters and setters
         */
        State state() const;
        void setState(const State &state);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade);

        quint64 funds() const;
        void setFunds(const quint64 &funds);

        quint64 priceIncrement() const;
        void setPriceIncrement(const quint64 &priceIncrement);

        quint32 index() const;
        void setIndex(const quint32 &index);

        KeyPair payorKeyPair() const;
        void setPayorKeyPair(const KeyPair &payorKeyPair);

        PublicKey payeeContractPk() const;
        void setPayeeContractPk(const PublicKey &payeeContractPk);

        PublicKey payeeFinalPk() const;
        void setPayeeFinalPk(const PublicKey &payeeFinalPk);

        Signature refund() const;
        void setRefund(const Signature &refund);

    private:

        // Slot state
        State _state;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Size of single payment
        quint64 _priceIncrement;

        // Output index
        quint32 _index;

        // Controls payour output of multisig
        KeyPair _payorKeyPair;

        // Controls payee output of multisig, from joinin_contract.pk
        PublicKey _payeeContractPk;

        // Controls payee payments, from sign_refund.pk
        PublicKey _payeeFinalPk;

        // Controls output refund
        Signature _refund;
    };

    // Default constructor
    PaymentChannelPayor();

    // Member wise constructor
    //PaymentChannelPayor(quint32 numberOfPayees, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);

    // Get contract for
    ContractTransaction getContract() const;

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Slot> _slots;

    // Unspent output funding channel
    OutputPoint _fundingOutput;

    // Controls output funding channel
    KeyPair _fundingOutputKeyPair;

    // Hash of contract
    Hash _contractHash;
};

#endif // PAYMENT_CHANNEL_PAYOR_HPP
