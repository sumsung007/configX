#ifndef NGINXWIDGET_H
#define NGINXWIDGET_H

#include "basewidget.h"

namespace Ui {
    class NginxWidget;
}

class NginxWidget: public BaseWidget
{
    Q_OBJECT

public:
    explicit NginxWidget(QWidget *parent = 0);
    ~NginxWidget();

private:
    Ui::NginxWidget *_ui;
};

#endif // NGINXWIDGET_H
