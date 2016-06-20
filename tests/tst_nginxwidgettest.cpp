#include <QtCore/QString>

#include <QtTest/QTest>

#include "../src/configeditor/modules/nginxwidget/nginxparser.hpp"
#include "mocksession.hpp"

class NginxWidgetTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testParser();
};

void NginxWidgetTest::testParser()
{
    MockSession *session = new MockSession;
    NginxParser *parser = new NginxParser(session);

    QVERIFY(parser->parse("/etc/nginx/nginx.conf"));
}

QTEST_APPLESS_MAIN(NginxWidgetTest)

#include "tst_nginxwidgettest.moc"
