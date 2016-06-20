#ifndef NETWORKSESSION_HPP
#define NETWORKSESSION_HPP

#include <QtCore/QString>

class NetworkSession
{
public:
    NetworkSession(QString host, int port);

    virtual bool readFileContents(const QString path, QString *content) = 0;

protected:
    QString host;
    int port;
};

#endif // NETWORKSESSION_HPP
