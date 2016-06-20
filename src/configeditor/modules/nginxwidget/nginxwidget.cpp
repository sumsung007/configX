#include "nginxwidget.hpp"
#include "ui_nginxwidget.h"

#include "../../configeditor.hpp"

#include <QtCore/QDebug>

NginxWidget::NginxWidget(NetworkSession *session, QWidget *parent):
    BaseWidget(editor, parent),
    _ui(new Ui::NginxWidget)
{
    qDebug() << "Create NginxWidget instance";

    this->_ui->setupUi(this);

    this->_parser = new NginxParser(session);
    this->_parser->parse("/etc/nginx/nginx.conf");
}

NginxWidget::~NginxWidget()
{
    delete this->_parser;
    delete this->_ui;

    qDebug() << "Destroy NginxWidget instance";
}
