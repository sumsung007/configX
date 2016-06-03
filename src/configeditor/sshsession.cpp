#include "sshsession.h"

#include <QtCore/QDebug>

SshSession::SshSession(QString host, int port, QString username, QString keyPath, QString keyPassphrase, QObject *parent):
    QObject(parent),
    _host(host),
    _port(port),
    _username(username),
    _keyPath(keyPath),
    _keyPassphrase(keyPassphrase)
{
    this->_session = ssh_new();
    if (this->_session == NULL) {
        qCritical("Could not create ssh session!");
        return;
    }

    uint connectionPort = (uint) this->_port;
    long timeout = 30;
    bool useSsh1 = false;
    bool useSsh2 = true;
    int logVerbosity = SSH_LOG_INFO;

    ssh_options_set(this->_session, SSH_OPTIONS_HOST, this->_host.toUtf8().constData());
    ssh_options_set(this->_session, SSH_OPTIONS_PORT, &connectionPort);
    ssh_options_set(this->_session, SSH_OPTIONS_USER, this->_username.toUtf8().constData());
    ssh_options_set(this->_session, SSH_OPTIONS_IDENTITY, this->_keyPath.toUtf8().constData());
    ssh_options_set(this->_session, SSH_OPTIONS_TIMEOUT, &timeout);
    ssh_options_set(this->_session, SSH_OPTIONS_SSH1, &useSsh1);
    ssh_options_set(this->_session, SSH_OPTIONS_SSH2, &useSsh2);
    ssh_options_set(this->_session, SSH_OPTIONS_LOG_VERBOSITY, &logVerbosity);

    int rc = ssh_connect(this->_session);
    if (rc != SSH_OK) {
        qWarning("Could not connect to ssh server: %s", ssh_get_error(this->_session));
        return;
    }

    this->_verifyKnownHost();
    this->_connectSsh();
}

SshSession::~SshSession()
{
    ssh_disconnect(this->_session);
    ssh_free(this->_session);
}

bool SshSession::_verifyKnownHost()
{
    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;

    state = ssh_is_server_known(this->_session);
    // TODO: Deprecated
    hlen = ssh_get_pubkey_hash(this->_session, &hash);

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

            if (ssh_write_knownhost(this->_session) < 0) {
                qCritical("Could not write key hash into known hosts file: %s", strerror(errno));
                free(hash);
                return false;
            }
            break;
        case SSH_SERVER_ERROR:
            qCritical("Could not verify host: %s", ssh_get_error(this->_session));
            free(hash);
            return false;
    }

    free(hash);
    return true;
}

bool SshSession::_connectSsh()
{
    int rc;
    rc = ssh_userauth_publickey_auto(this->_session, NULL, this->_keyPassphrase.toUtf8().constData());
    if (rc == SSH_AUTH_ERROR) {
        qCritical("Authentication failed: %s", ssh_get_error(this->_session));

        return false;
    }

    return true;
}
