#ifndef SSHSESSION_H
#define SSHSESSION_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>

#include <libssh/libssh.h>
#include <libssh/sftp.h>

class SshSession: public QObject
{
    Q_OBJECT

public:
    explicit SshSession(QString host, int port, QString username, QString keyPath, QString keyPassphrase, QObject *parent = 0);
    ~SshSession();

    bool readFileContents(const QString path, QString *content);
    bool executeCommand(QString command, QString *result);

    QDateTime uptime() const;
    bool isConnected() const;

protected:
    ssh_channel openChannel();
    void closeChannel(ssh_channel channel);

    void updateTime();

private:
    QString _host;
    int _port;
    QString _username;
    QString _keyPath;
    QString _keyPassphrase;

    QTime _serverTime;
    QDateTime _uptime;

    ssh_session _sshSession;
    sftp_session _sftpSession;
    bool _isConnected;

    bool _verifyKnownHost();
    bool _authSsh();
    bool _connectSftp();
};

#endif // SSHSESSION_H
