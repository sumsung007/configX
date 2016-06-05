#include "configeditor.h"
#include "ui_configeditor.h"

#include "modules/nginxwidget.h"

#include <QtCore/QDebug>

ConfigEditor::ConfigEditor(ServerTreeItem *item, QWidget *parent):
    QWidget(parent),
    _ui(new Ui::ConfigEditor),
    _item(item)
{
    qDebug() << "Create ConfigEditor instance";

    // Connect ssh session
    this->_sshSession = new SshSession(item->host(), item->port(), item->username(), item->keyPath(), item->keyPassphrase(), this);

    // Setup UI
    this->_ui->setupUi(this);
    if (this->_sshSession->isConnected()) {
        this->_ui->statusLabel->setText("Online");
        this->_ui->uptimeLabel->setText(this->_sshSession->uptime().toString("yyyy/MM/dd hh:mm:ss"));
    }

    this->_ui->moduleWidget->addWidget(new NginxWidget(this, this));

    connect(this->_ui->nginxButton, &QToolButton::clicked, [this]() {
        this->_ui->nginxButton->setChecked(true);
        this->_ui->moduleWidget->setCurrentIndex(0);
    });

    this->_ui->moduleWidget->setCurrentIndex(-1);

    // Set splitter size
    QList<int> splitterSizes;
    splitterSizes << this->width() / 4 * 1 << this->width() / 4 * 3;
    this->_ui->splitter->setSizes(splitterSizes);
    this->_ui->splitter->setStretchFactor(0, 1);
    this->_ui->splitter->setStretchFactor(1, 3);
}

ConfigEditor::~ConfigEditor()
{
    delete this->_ui;

    qDebug() << "Destroy ConfigEditor instance";
}

ServerTreeItem *ConfigEditor::item() const
{
    return this->_item;
}
