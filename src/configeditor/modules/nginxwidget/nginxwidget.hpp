#ifndef NGINXWIDGET_H
#define NGINXWIDGET_H

#include "../basewidget.hpp"
#include "nginxparser.hpp"

#include <QtCore/QVariant>

namespace Ui {
    class NginxWidget;
}

class NginxWidget: public BaseWidget
{
    Q_OBJECT

public:
    explicit NginxWidget(NetworkSession *session, QWidget *parent = 0);
    ~NginxWidget();

protected:
    void parseConfig(const QString path);

private:
    Ui::NginxWidget *_ui;
    NginxParser *_parser;
};

#endif // NGINXWIDGET_H
