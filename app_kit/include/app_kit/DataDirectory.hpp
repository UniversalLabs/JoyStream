/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_DATADIRECTORY_HPP
#define JOYSTREAM_APPKIT_DATADIRECTORY_HPP

#include <QString>
#include <QDir>
#include <QLockFile>

namespace joystream {
namespace appkit {

class DataDirectory
{
public:
    static const QString LockFileName;

    DataDirectory(const std::string &dir);

    void lock();

    void unlock();

    QLockFile::LockError lockError() const;

    QString walletPath() const;
    QString walletFilePath() const;
    QString blockTreeFilePath() const;
    QString defaultSavePath() const;
    QString savedTorrentsFilePath() const;

    void makeDirs() const;

private:
    QDir _dir;
    QLockFile _lockFile;
};

} // appkit
} // joystream

#endif // JOYSTREAM_APPKIT_DATADIRECTORY_HPP
