#ifndef SSHSESSION_H
#define SSHSESSION_H

#include <QtCore/QObject>

#include <libssh/libssh.h>

class SshSession: public QObject
{
    Q_OBJECT

public:
    explicit SshSession(QString host, int port, QString username, QString keyPath, QString keyPassphrase, QObject *parent = 0);
    ~SshSession();

private:
    QString _host;
    int _port;
    QString _username;
    QString _keyPath;
    QString _keyPassphrase;

    ssh_session _session;

    bool _verifyKnownHost();
    bool _connectSsh();
};

#endif // SSHSESSION_H
