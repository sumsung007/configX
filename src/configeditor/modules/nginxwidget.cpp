#include "nginxwidget.h"
#include "ui_nginxwidget.h"

#include <QtCore/QDebug>

NginxWidget::NginxWidget(QWidget *parent):
    BaseWidget(parent),
    _ui(new Ui::NginxWidget)
{
    qDebug() << "Create NginxWidget instance";

    this->_ui->setupUi(this);
}

NginxWidget::~NginxWidget()
{
    delete this->_ui;

    qDebug() << "Destroy NginxWidget instance";
}
