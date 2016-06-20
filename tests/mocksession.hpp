#ifndef MOCKSESSION_HPP
#define MOCKSESSION_HPP

#include "../src/configeditor/networksession.hpp"

#include <QtCore/QString>

class MockSession: public NetworkSession
{
public:
    MockSession();

    bool readFileContents(const QString path, QString *content);
};

#endif // MOCKSESSION_HPP
