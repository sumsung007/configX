#include "servertree/servertreeitem.hpp"
#include "servertree/serveritemdialog.h"
#include "configeditor/configeditor.h"

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    qDebug() << "Create MainWindow instance";

    this->_ui->setupUi(this);

    this->loadServerTree();
    this->_ui->serverTree->expandAll();

    connect(this->_ui->addFolderButton, &QPushButton::clicked, [this]() {
        this->addServerTreeItem(true);
    });

    connect(this->_ui->addServerButton, &QPushButton::clicked, [this]() {
        this->addServerTreeItem(false);
    });

    connect(this->_ui->editButton, &QPushButton::clicked, [this]() {
        if (this->_ui->serverTree->selectedItems().count() <= 0)
            return;

        QTreeWidgetItem *item = this->_ui->serverTree->selectedItems().first();
        ServerTreeItem *server = dynamic_cast<ServerTreeItem*>(item);
        if (server->isCategory()) {
            this->_ui->serverTree->editItem(item, 0);
            return;
        }

        ServerItemDialog dlg(server, this);
        dlg.exec();

        this->saveServerTree();
    });

    connect(this->_ui->serverTree, &ServerTreeWidget::itemChanged, [this]() {
        this->saveServerTree();
    });

    connect(this->_ui->serverTree, &ServerTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column) {
        Q_UNUSED(column);

        ServerTreeItem *server = dynamic_cast<ServerTreeItem*>(item);
        if (!server) {
            qCritical("Could not convert tree item to server tree item");
            return;
        }

        if (server->isCategory())
            return;

        // Check if tab is already open and switch to it
        if (server->openTab() != Q_NULLPTR) {
            this->_ui->configTabWidget->setCurrentWidget(server->openTab());
            return;
        }

        // Create new tab
        ConfigEditor *configEditor = new ConfigEditor(server);
        this->_ui->configTabWidget->addTab(configEditor, server->text(ServerTreeItem::COLUMN_NAME));
        server->setOpenTab(configEditor);
    });
}

MainWindow::~MainWindow()
{
    delete this->_ui;

    qDebug() << "Destroy MainWindow instance";
}

bool MainWindow::saveServerTree()
{
    qDebug() << "Saving server tree to file";

    // Update library with user data
    QJsonObject config = this->_config;
    QTreeWidgetItem *root = this->_ui->serverTree->invisibleRootItem();
    QJsonArray items;
    for (int i = 0; i < root->childCount(); i++) {
        ServerTreeItem *server = dynamic_cast<ServerTreeItem*>(root->child(i));
        if (!server) {
            qCritical("Could not convert tree item to note tree item");
            return false;
        }

        items.append(server->toJson());
    }
    config.insert("library", items);
    this->_config = config;

    // Create new json document
    QJsonDocument doc;
    doc.setObject(config);

    QFile configFile(this->_getConfigPath());
    if (!configFile.open(QIODevice::WriteOnly)) {
        qCritical("Could not open config file %s in write only mode", this->_getConfigPath().toUtf8().constData());
        return false;
    }

    return (configFile.write(doc.toJson()) != -1);
}

void MainWindow::loadServerTree()
{
    qDebug() << "Loading server tree from file";

    QFile configFile(this->_getConfigPath());
    if (!configFile.exists()) {
        qWarning("Config file %s does not exist yet.", configFile.fileName().toUtf8().constData());
        return;
    }

    if (!configFile.open(QIODevice::ReadOnly)) {
        qFatal("Could not open config file %s in read only mode!", configFile.fileName().toUtf8().constData());
        exit(EXIT_FAILURE);
    }
    QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
    if (doc.isObject())
        this->_config = doc.object();
    else
        this->_config = QJsonObject();

    QJsonArray servers;
    if (this->_config.value("library").isArray()) {
        servers = this->_config.value("library").toArray();
    } else {
        qWarning("'library' is not an array!");
        this->_config.insert("library", QJsonArray());
    }

    QTreeWidgetItem *root = this->_ui->serverTree->invisibleRootItem();

    for (int i = 0; i < servers.count(); i++) {
        QJsonObject child;
        if (servers.at(i).isObject())
            child = servers.at(i).toObject();

        ServerTreeItem::ItemType type;
        if (child.value("category").toBool(false))
            type = ServerTreeItem::CategoryType;
        else
            type = ServerTreeItem::ServerType;

        root->addChild(new ServerTreeItem(child, type));
    }
}

void MainWindow::addServerTreeItem(bool isCategory)
{
    QString defaultTitle;
    ServerTreeItem::ItemType type;

    if (isCategory) {
        qDebug() << "Add new category";
        defaultTitle = tr("Category");
        type = ServerTreeItem::CategoryType;
    } else {
        qDebug() << "Add new server";
        defaultTitle = tr("Server");
        type = ServerTreeItem::ServerType;
    }

    ServerTreeItem *item = new ServerTreeItem(defaultTitle, type);
    QTreeWidgetItem *parent;
    if (this->_ui->serverTree->selectedItems().count() > 0) {
        parent = this->_ui->serverTree->selectedItems().first();

        if (parent->type() != ServerTreeItem::CategoryType)
            parent = parent->parent();
    } else {
        parent = this->_ui->serverTree->invisibleRootItem();
    }

    parent->addChild(item);
    parent->setExpanded(true);

    this->saveServerTree();
}

QString MainWindow::_getConfigPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).append(QDir::separator()).append("configX.json");
}
