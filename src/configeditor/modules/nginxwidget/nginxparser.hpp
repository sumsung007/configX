#ifndef NGINXPARSER_HPP
#define NGINXPARSER_HPP

#include "configsection.hpp"

#include <QtCore/QString>

class NetworkSession;

class NginxParser
{
public:
    NginxParser(NetworkSession *session);

    bool parse(QString path, ConfigSection *parent = Q_NULLPTR);

private:
    NetworkSession *_session;
};

#endif // NGINXPARSER_HPP
