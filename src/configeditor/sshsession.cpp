#include "sshsession.hpp"

#include <fcntl.h>

#include <QtCore/QDebug>

SshSession::SshSession(QString host, int port, QString username, QString keyPath, QString keyPassphrase):
    NetworkSession(host, port),
    _username(username),
    _keyPath(keyPath),
    _keyPassphrase(keyPassphrase)
{
    this->_sshSession = ssh_new();
    if (this->_sshSession == NULL) {
        qCritical("Could not create ssh session!");
        return;
    }

    this->_sftpSession = NULL;

    uint connectionPort = (uint) this->port;
    long timeout = 10;
    bool useSsh1 = false;
    bool useSsh2 = true;
    int logVerbosity = SSH_LOG_INFO;

    ssh_options_set(this->_sshSession, SSH_OPTIONS_HOST, this->host.toUtf8().constData());
    ssh_options_set(this->_sshSession, SSH_OPTIONS_PORT, &connectionPort);
    ssh_options_set(this->_sshSession, SSH_OPTIONS_USER, this->_username.toUtf8().constData());
    ssh_options_set(this->_sshSession, SSH_OPTIONS_IDENTITY, this->_keyPath.toUtf8().constData());
    ssh_options_set(this->_sshSession, SSH_OPTIONS_TIMEOUT, &timeout);
    ssh_options_set(this->_sshSession, SSH_OPTIONS_SSH1, &useSsh1);
    ssh_options_set(this->_sshSession, SSH_OPTIONS_SSH2, &useSsh2);
    ssh_options_set(this->_sshSession, SSH_OPTIONS_LOG_VERBOSITY, &logVerbosity);

    int rc = ssh_connect(this->_sshSession);
    if (rc != SSH_OK) {
        qWarning("Could not connect to ssh server: %s", ssh_get_error(this->_sshSession));
        return;
    }

    // Verify host and connect
    if (this->_verifyKnownHost()) {
        this->_isConnected = this->_authSsh() && this->_connectSftp();
    } else {
        qWarning("Could not verfiy host!");
        this->_isConnected = false;
        return;
    }

    this->updateTime();
}

SshSession::~SshSession()
{
    if (this->_sftpSession != NULL)
        sftp_free(this->_sftpSession);

    if (ssh_is_connected(this->_sshSession))
        ssh_disconnect(this->_sshSession);

    if (this->_sshSession != NULL)
        ssh_free(this->_sshSession);

    qDebug() << "Destroy ssh session";
}

bool SshSession::readFileContents(const QString path, QString *content)
{
    qDebug() << "Read contents of file" << path;

    if (!this->_isConnected) {
        qCritical("Not connected to ssh server!");
        return false;
    }

    int access_type;
    sftp_file file;
    char buffer[16384];
    int nbytes;
    int rc;

    *content = "";
    access_type = O_RDONLY;

    // Open file read only
    file = sftp_open(this->_sftpSession, path.toUtf8().constData(), access_type, 0);
    if (file == NULL) {
        qCritical("Could not open file %s for reading: %d", path.toUtf8().constData(), sftp_get_error(this->_sftpSession));
        return false;
    }

    // Read contents of file
    for (;;) {
        nbytes = sftp_read(file, buffer, sizeof(buffer));

        if (nbytes == 0) {
            break; // EOF
        } else if (nbytes < 0) {
            qCritical("Error while reading file %s: %d", path.toUtf8().constData(), sftp_get_error(this->_sftpSession));
            sftp_close(file);
            *content = "";

            return false;
        }

        content->append(buffer);
    }

    // Close file
    rc = sftp_close(file);
    if (rc != SSH_OK) {
        qCritical("Could not close file %s: %d", path.toUtf8().constData(), sftp_get_error(this->_sftpSession));
        return rc;
    }

    return true;
}

bool SshSession::executeCommand(QString command, QString *result)
{
    if (!this->_isConnected) {
        qCritical("Not connected to ssh server!");
        return false;
    }

    command.prepend("bash -c ");
    qDebug() << "Execute command" << command;

    ssh_channel channel = this->openChannel();
    if (channel == NULL)
        return false;

    // Execute command
    int rc = ssh_channel_request_exec(channel, command.toUtf8().constData());
    if (rc != SSH_OK) {
        qCritical("Could not execute remote command (%d): %s", rc, command.toUtf8().constData());
        this->closeChannel(channel);

        return false;
    }

    // Read return value
    char buffer[256];
    int nbytes;
    QString tempResult;

    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        tempResult.append(buffer);

        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0) {
        this->closeChannel(channel);
        return false;
    }

    *result = tempResult.trimmed();
    return true;
}

QDateTime SshSession::uptime() const
{
    return this->_uptime;
}

bool SshSession::isConnected() const
{
    return this->_isConnected;
}

ssh_channel SshSession::openChannel()
{
    ssh_channel channel;
    int rc;
    channel = ssh_channel_new(this->_sshSession);
    if (channel == NULL)  {
        qCritical("Could not create ssh channel!");
        return NULL;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        qCritical("Could not open ssh channel for session!");

        ssh_channel_free(channel);
        return NULL;
    }

    return channel;
}

void SshSession::closeChannel(ssh_channel channel)
{
    ssh_channel_close(channel);
    ssh_channel_send_eof(channel);
    ssh_channel_free(channel);
}

void SshSession::updateTime()
{
    QString uptime;

    // Read uptime
    if (!this->executeCommand("uptime", &uptime)) {
        qCritical("Could not get uptime from server!");
        return;
    }

    qDebug() << "Uptime" << uptime;

    QRegExp regexMinute("(\\d+:\\d+:\\d+)\\s+up\\s+(\\d+\\:\\d+|\\d+).*,\\s+(\\d+)\\s+user[s]?,\\s+load average:\\s+(\\d+[\\.|,]\\d+),\\s+(\\d+[\\.|,]\\d+),\\s+(\\d+[\\.|,]\\d+).*");
    QRegExp regexDay("(\\d+:\\d+:\\d+)\\s+up\\s+(\\d+|\\d+:\\d+)\\s+days?,\\s+(\\d+ min|\\d+:\\d+)?,\\s+(\\d+)\\s+user[s]?,\\s+load average:\\s+(\\d+\\.\\d+),\\s+(\\d+\\.\\d+),\\s+(\\d+\\.\\d+).*");

    if (regexDay.indexIn(uptime) > -1) {
        qDebug() << "Running >= 24h";

        this->_serverTime = QTime::fromString(regexDay.cap(1), "hh:mm:ss");
        qDebug() << "Server time" << this->_serverTime;

        this->_uptime = QDateTime::currentDateTime();
        this->_uptime.setTime(this->_serverTime);

        int days = regexDay.cap(2).toInt();

        QString running = regexDay.cap(3);
        qDebug() << "Running" << running;
        if (running.contains(":")) {
            int hours = running.left(running.indexOf(":")).toInt();
            int minutes = running.right(2).toInt();
            qDebug() << "Hours/minutes/days" << hours << minutes << days;

            this->_uptime = this->_uptime.addSecs((-1) * ((days * 24 * 60 * 60) + (hours * 60 * 60) + (minutes * 60)));
        } else {
            this->_uptime = this->_uptime.addSecs((-1) * (running.toInt() * 60));
        }
    } else if (regexMinute.indexIn(uptime) > -1) {
        qDebug() << "Running < 24h";

        this->_serverTime = QTime::fromString(regexMinute.cap(1), "hh:mm:ss");
        qDebug() << "Server time" << this->_serverTime;

        this->_uptime = QDateTime::currentDateTime();
        this->_uptime.setTime(this->_serverTime);

        QString running = regexMinute.cap(2);
        qDebug() << "Running" << running;
        if (running.contains(":")) {
            int hours = running.left(running.indexOf(":")).toInt();
            int minutes = running.right(2).toInt();
            qDebug() << "Hours/minutes" << hours << minutes;

            this->_uptime = this->_uptime.addSecs((-1) * ((hours * 60 * 60) + (minutes * 60)));
        } else {
            this->_uptime = this->_uptime.addSecs((-1) * (running.toInt() * 60));
        }
    } else {
        qWarning("Invalid uptime format: %s", uptime.toUtf8().constData());
        return;
    }
}

bool SshSession::_verifyKnownHost()
{
    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;

    state = ssh_is_server_known(this->_sshSession);
    // TODO: Deprecated
    hlen = ssh_get_pubkey_hash(this->_sshSession, &hash);

    if (hlen < 0)
        return false;

    switch (state) {
        case SSH_SERVER_KNOWN_OK:
            // Server is in known hosts file
            break;
        case SSH_SERVER_KNOWN_CHANGED:
            hexa = ssh_get_hexa(hash, hlen);
            qWarning("Host key for server changed: it is now: %s", hexa);
            qWarning("For security reasons, connection will be stopped!");
            free(hash);
            return false;
        case SSH_SERVER_FOUND_OTHER:
            qCritical("The host key for this server was not found but an other type of key exists! An attacker might change the default server key to confuse your client into thinking the key does not exist.");
            free(hash);
            return false;
        case SSH_SERVER_FILE_NOT_FOUND:
            qWarning("Could not find known host file! If you accept the host key here, the file will be automatically created.\n");
            // fallback to SSH_SERVER_NOT_KNOWN behavior
        case SSH_SERVER_NOT_KNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            // TODO: Ask user
            qWarning("The server is unknown and will be automatically trusted.");
            qWarning("Public key hash: %s\n", hexa);
            free(hexa);

            if (ssh_write_knownhost(this->_sshSession) < 0) {
                qCritical("Could not write key hash into known hosts file: %s", strerror(errno));
                free(hash);
                return false;
            }
            break;
        case SSH_SERVER_ERROR:
            qCritical("Could not verify host: %s", ssh_get_error(this->_sshSession));
            free(hash);
            return false;
    }

    free(hash);
    return true;
}

bool SshSession::_authSsh()
{
    int rc;
    /*rc = ssh_userauth_publickey_auto(this->_sshSession, NULL, this->_keyPassphrase.toUtf8().constData());
    if (rc != SSH_AUTH_SUCCESS) {
        qCritical("Authentication failed: %s", ssh_get_error(this->_sshSession));

        return false;
    }*/

    ssh_key privateKey;
    rc = ssh_pki_import_privkey_file(this->_keyPath.toUtf8().constData(), this->_keyPassphrase.toUtf8().constData(), NULL, NULL, &privateKey);
    if (rc != SSH_OK) {
        qCritical("Could not load private key from %s: %s", this->_keyPath.toUtf8().constData(), ssh_get_error(this->_sshSession));
        ssh_key_free(privateKey);

        return false;
    }

    rc = ssh_userauth_publickey(this->_sshSession, NULL, privateKey);
    if (rc != SSH_AUTH_SUCCESS) {
        qCritical("Could not authenticate with private key from %s: %s", this->_keyPath.toUtf8().constData(), ssh_get_error(this->_sshSession));
        ssh_key_free(privateKey);

        return false;
    }

    ssh_key_free(privateKey);

    return true;
}

bool SshSession::_connectSftp()
{
    int rc;

    this->_sftpSession = sftp_new(this->_sshSession);
    if (this->_sftpSession == NULL) {
        qCritical("Error allocating SFTP session: %s", ssh_get_error(this->_sshSession));
        return false;
    }

    rc = sftp_init(this->_sftpSession);
    if (rc != SSH_OK) {
        qCritical("Error initializing SFTP session: %d", sftp_get_error(this->_sftpSession));
        sftp_free(this->_sftpSession);
        return rc;
    }

    return true;
}
