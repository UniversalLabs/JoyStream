#include "WalletDialog.hpp"
#include "ui_walletdialog.h"

#include <QInputDialog>
#include <QMessageBox>

#include "extension/BitCoin/Wallet.hpp"

//WalletDialog::WalletDialog(QWidget *parent, Wallet * wallet) {}

WalletDialog::WalletDialog(Wallet * wallet) :
    ui(new Ui::WalletDialog),
    _wallet(wallet) {
    ui->setupUi(this);

    // Setup View model
    this->setWindowTitle("Wallet");

    // Add columns to model view model
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Date"));
    _walletTableViewModel.setHorizontalHeaderItem(1, new QStandardItem("Type"));
    _walletTableViewModel.setHorizontalHeaderItem(2, new QStandardItem("Value"));
    _walletTableViewModel.setHorizontalHeaderItem(3, new QStandardItem("#Confirmations"));

    // Set table model to view model
    ui->walletTableView->setModel(&_walletTableViewModel);

    // Populate view
    refresh();
}

void WalletDialog::refresh() {

    // Clear view model
    clearWalletTableView();

    // Update view model
    updateWalletTableView();

    // Get number of keys in wallet
    int numberOfKeys = _wallet->numberOfKeysInWallet();
    ui->numKeys->setText(QString("#Keys: ") + QString::number(numberOfKeys));

    // Get number of transactions in wallet
    int numberOfTransactions = _wallet->numberOfTransactions();
    ui->numTransactions->setText(QString("#Transactions: ") + QString::number(numberOfTransactions));
}

void WalletDialog::clearWalletTableView() {

    int rowCount = _walletTableViewModel.rowCount();

    for(int i = rowCount-1;i >= 0;i--) {

        // Get list
        QList<QStandardItem *> & list = _walletTableViewModel.takeRow(i);

        for(QList<QStandardItem *>::iterator itr = list.begin();
            itr != list.end();itr++)
            delete *itr;
    }
}

void WalletDialog::updateWalletTableView() {

    // Get wallet entries
    const QMap<PublicKey, Wallet::Entry> & entries = _wallet->entries();

    // List of all events
    QList<Wallet::TxOEvent> events;

    // Iterate wallet entries
    for(QMap<PublicKey, Wallet::Entry>::const_iterator i = entries.constBegin();
        i != entries.constEnd();i++) {

        // Get entry
        const Wallet::Entry & entry = i.value();

        // For each entry, iterate entry events (send and receive)

        // send
        const QMap<OutPoint, Wallet::TxOEvent> & send = entry.send();
        for(QMap<OutPoint, Wallet::TxOEvent>::const_iterator j = send.constBegin();
            j != send.constEnd();j++)
            events.append(j.value());

        // receive
        const QMap<OutPoint, Wallet::TxOEvent> & receive = entry.receive();
        for(QMap<OutPoint, Wallet::TxOEvent>::const_iterator j = receive.constBegin();
            j != receive.constEnd();j++)
            events.append(j.value());
    }

    // Sort events
    qSort(events.begin(), events.end());

    // Iterate and add to table view model
    for(QList<Wallet::TxOEvent>::const_iterator i = events.constBegin();
        i != events.constEnd();i++)
        _walletTableViewModel.appendRow(toModelViewRow(*i));

    /**
    // Get wallet entries
    const QMap<PublicKey, Wallet::Entry> & entries = _wallet->entries();

    // Iterate entries
    for(QMap<PublicKey, Wallet::Entry>::const_iterator i = entries.constBegin();
        i != entries.constEnd();i++) {

        // Get entry for key
        const Wallet::Entry & entry = i.value();

        // Get outputs in entry
        const QMap<OutPoint, Wallet::Entry::Output> & outputs = entry.outputs();

        for(QMap<OutPoint, Wallet::Entry>::const_iterator i = outputs.constBegin();
            i != outputs.constEnd();i++) {

            // Get output
            const Wallet::Entry::Output & output = i.value();

            // Create new row
            QList<QStandardItem *> items;

            // Date
            items << new QStandardItem(output.added().toString());

            // Type
            QString type;
            switch(output.type()) {


        case Wallet::Purpose::Receive: type = "Receive"; break;
        case Wallet::Purpose::ContractChange: type = "Contract change to buyer"; break;
        case Wallet::Purpose::BuyerInContractOutput: type = "Contract output to buyer"; break;
        case Wallet::Purpose::ContractFinal: type = "Contract output change/refund to buyer"; break;
        case Wallet::Purpose::SellerInContractOutput: type = "Contract output to seller"; break;
        case Wallet::Purpose::ContractPayment: type = "Payment to seller"; break;
            }

            items << new QStandardItem(type);

            // State
            QString state;
            switch(output.state()) {

                case Wallet::Entry::Output::State::Free: state = "Free";
                case Wallet::Entry::Output::State::Locked: state = "Locked";
            }

            items << new QStandardItem(state);

            // Description
            items << new QStandardItem(entry.description());

            // Value
            items << new QStandardItem(output.value());

            // Confirmations
            items << new QStandardItem(_wallet->latestBlockHeight() - output.blockHeight());

            // Spent
            items << new QStandardItem(output.spent() ? "True" : "False");

            // Added to view model
            _walletTableViewModel.appendRow(items);
        }
    }

    */

}

QList<QStandardItem *> WalletDialog::toModelViewRow(const Wallet::TxOEvent & event) const {

    // Create new row
    QList<QStandardItem *> items;

    // Date
    items << new QStandardItem(event.firstSeen().toString());

    // Type
    QString type;
    switch(event.type()) {

        case Wallet::TxOEvent::Type::Send: type = "Sending"; break;
        case Wallet::TxOEvent::Type::Receive: type = "Receive"; break;
    }

    items << new QStandardItem(type);

    // Value
    QString value = QString::number(event.value());
    items << new QStandardItem(value);

    // Confirmations
    QString confirmations;

    if(event.blockHeight() == 0)
        confirmations = "Unconfirmed";
    else
        confirmations = QString::number(_wallet->latestBlockHeight() - event.blockHeight());

    items << new QStandardItem(confirmations);

    return items;
}

WalletDialog::~WalletDialog() {
    delete ui;
}

void WalletDialog::on_receivePushButton_clicked() {

    // Get description
    bool ok;

    QString description = QInputDialog::getText(this,
                                                tr("QInputDialog::getText()"),
                                                tr("Description:"),
                                                QLineEdit::Normal,
                                                "",
                                                &ok);

    // Check that description was provided
    if(!ok || description.isEmpty())
        return;

    // Get wallet entry
    Wallet::Entry entry = _wallet->addReceiveKey(description, Wallet::Purpose::Receive);

    // Get address
    QString address = _wallet->toAddress(entry.keyPair().pk());

    // Show address in message box
    QMessageBox msgBox;

    msgBox.setText("Added: " + address);
    msgBox.setDetailedText(address);
    // + " : " + entry.keyPair().pk().toString() + " : " + entry.keyPair().sk().toString() );
    msgBox.exec();

    // Update view model
    refresh();
}

void WalletDialog::on_synchronizePushButton_clicked() {

    // Synch wallet
    _wallet->synchronize();

    // Update view model
    refresh();
}