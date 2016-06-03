#include "configeditor.h"
#include "ui_configeditor.h"

#include <QtCore/QDebug>

ConfigEditor::ConfigEditor(ServerTreeItem *item, QWidget *parent):
    QWidget(parent),
    _ui(new Ui::ConfigEditor),
    _item(item)
{
    qDebug() << "Create ConfigEditor instance";

    this->_ui->setupUi(this);
}

ConfigEditor::~ConfigEditor()
{
    delete this->_ui;

    qDebug() << "Destroy ConfigEditor instance";
}
