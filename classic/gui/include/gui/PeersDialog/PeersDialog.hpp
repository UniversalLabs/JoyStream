/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 23 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP

#include <libtorrent/socket_io.hpp>

#include <QDialog>

class QStandardItemModel;

namespace Ui {
    class PeersDialog;
}

namespace joystream {
namespace classic {
namespace gui {

class PeersDialog : public QDialog
{
    Q_OBJECT

public:

    explicit PeersDialog(QWidget * parent);

    ~PeersDialog();

    void setPeerTreeViewModel(QStandardItemModel * model);
    void setObserverConnectionTreeViewModel(QStandardItemModel * model);
    void setBuyerConnectionTreeViewModel(QStandardItemModel * model);
    void setSellerConnectionTreeViewModel(QStandardItemModel * model);

private:

    Ui::PeersDialog * ui;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP
