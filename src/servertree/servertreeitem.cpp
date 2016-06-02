#include "servertreeitem.hpp"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>

ServerTreeItem::ServerTreeItem(QJsonObject item, ServerTreeItem::ItemType type):
    QTreeWidgetItem(type),
    _editor(Q_NULLPTR)
{
    qDebug() << "Create existing ServerTreeItem instance";

    this->setText(ServerTreeItem::COLUMN_NAME, item.value("name").toString(""));

    if (!item.value("category").toBool()) {
        // Set server properties

        this->_host = item.value("host").toString();
        this->_port = item.value("port").toInt();
        this->_username = item.value("username").toString();
        this->_keyPath = item.value("keyPath").toString();
        this->_keyPassphrase = item.value("keyPassphrase").toString();
    } else {
        // Add category children

        QJsonArray children;
        if (item.value("servers").isArray())
            children = item.value("servers").toArray();

        for (int i = 0; i < children.count(); i++) {
            QJsonObject child;
            if (children.at(i).isObject())
                child = children.at(i).toObject();

            ServerTreeItem::ItemType type;
            if (child.value("category").toBool(false))
                type = ServerTreeItem::CategoryType;
            else
                type = ServerTreeItem::ServerType;

            this->addChild(new ServerTreeItem(child, type));
        }
    }

    if (this->type() == ServerTreeItem::CategoryType)
        this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
    else if (this->type() == ServerTreeItem::ServerType)
        this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
}

ServerTreeItem::ServerTreeItem(QString name, ServerTreeItem::ItemType type):
    QTreeWidgetItem(type),
    _editor(Q_NULLPTR)
{
    qDebug() << "Create empty ServerTreeItem instance";

    this->setText(0, name);
    this->setPort(22);

    if (this->type() == ServerTreeItem::CategoryType)
        this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
    else if (this->type() == ServerTreeItem::ServerType)
        this->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
}

ServerTreeItem::~ServerTreeItem()
{
    qDebug() << "Destroy ServerTreeItem instance";
}

bool ServerTreeItem::isCategory()
{
    return (this->type() == ServerTreeItem::CategoryType);
}

void ServerTreeItem::setOpenTab(QWidget *editor)
{
    this->_editor = editor;
}

QWidget* ServerTreeItem::openTab()
{
    return this->_editor;
}

QJsonObject ServerTreeItem::toJson() const
{
    QJsonObject item;
    item.insert("name", this->text(ServerTreeItem::COLUMN_NAME));
    item.insert("category", (this->type() == ServerTreeItem::CategoryType) ? true : false);

    if (this->type() == ServerTreeItem::ServerType) {
        qDebug() << "Save server";
        item.insert("host", this->_host);
        item.insert("port", this->_port);
        item.insert("username", this->_username);
        item.insert("keyPath", this->_keyPath);
        item.insert("keyPassphrase", this->_keyPassphrase);

        return item;
    }

    qDebug() << "Save category";

    // Append children
    QJsonArray children;
    for (int i = 0; i < this->childCount(); i++) {
        ServerTreeItem *note = dynamic_cast<ServerTreeItem*>(this->child(i));
        children.append(note->toJson());
    }

    item.insert("servers", children);
    return item;
}

QString ServerTreeItem::host() const
{
    return this->_host;
}

void ServerTreeItem::setHost(const QString &host)
{
    this->_host = host;
}

int ServerTreeItem::port() const
{
    return this->_port;
}

void ServerTreeItem::setPort(const int &port)
{
    this->_port = port;
}

QString ServerTreeItem::username() const
{
    return this->_username;
}

void ServerTreeItem::setUsername(const QString &username)
{
    this->_username = username;
}

QString ServerTreeItem::keyPath() const
{
    return this->_keyPath;
}

void ServerTreeItem::setKeyPath(const QString &keyPath)
{
    this->_keyPath = keyPath;
}

QString ServerTreeItem::keyPassphrase() const
{
    return this->_keyPassphrase;
}

void ServerTreeItem::setKeyPassphrase(const QString &keyPassphrase)
{
    this->_keyPassphrase = keyPassphrase;
}
