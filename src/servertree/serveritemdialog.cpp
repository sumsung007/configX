#include "serveritemdialog.h"
#include "ui_serveritemdialog.h"

#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>

#include <QtWidgets/QFileDialog>

ServerItemDialog::ServerItemDialog(ServerTreeItem *item, QWidget *parent):
    QDialog(parent),
    _ui(new Ui::ServerItemDialog),
    _item(item)
{
    qDebug() << "Create ServerItemDialog instance";

    this->_ui->setupUi(this);

    this->setWindowTitle(item->text(0));

    this->_ui->hostEdit->setText(item->host());
    this->_ui->portSpinBox->setValue(item->port());
    this->_ui->usernameEdit->setText(item->username());
    this->_ui->keyPathEdit->setText(item->keyPath());
    this->_ui->keyPassphraseEdit->setText(item->keyPassphrase());

    connect(this->_ui->chooseKeyButton, &QPushButton::clicked, [this]() {
        QString path = QFileDialog::getOpenFileName(this, tr("Choose private key"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        if (path.isEmpty())
            return;

        this->_ui->keyPathEdit->setText(path);
    });

    connect(this->_ui->abortButton, &QPushButton::clicked, this, &ServerItemDialog::close);
    connect(this->_ui->saveButton, &QPushButton::clicked, [this]() {
        this->_item->setHost(this->_ui->hostEdit->text());
        this->_item->setPort(this->_ui->portSpinBox->value());
        this->_item->setUsername(this->_ui->usernameEdit->text());
        this->_item->setKeyPath(this->_ui->keyPathEdit->text());
        this->_item->setKeyPassphrase(this->_ui->keyPassphraseEdit->text());

        this->close();
    });
}

ServerItemDialog::~ServerItemDialog()
{
    delete this->_ui;

    qDebug() << "Destroy ServerItemDialog instance";
}
