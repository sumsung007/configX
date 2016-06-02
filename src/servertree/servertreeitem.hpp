#ifndef NOTETREEITEM_HPP
#define NOTETREEITEM_HPP

#include <QtCore/QJsonObject>

#include <QtWidgets/QTreeWidgetItem>

class ServerTreeItem;

class ServerTreeItem: public QTreeWidgetItem
{
public:
    enum ItemType {
        Type = 0,
        UserType = 1000,
        CategoryType = 1100,
        ServerType = 1200
    };

    static const int COLUMN_NAME = 0;

    /**
     * @brief Child item constructor for existing items
     * @param item
     */
    ServerTreeItem(QJsonObject item, ServerTreeItem::ItemType type);

    /**
     * @brief Child item constructor for new items
     * @param name
     */
    ServerTreeItem(QString name, ServerTreeItem::ItemType type);

    ~ServerTreeItem();

    Qt::ItemFlags flags() const;

    /**
     * @brief Check if item is a category
     * @return
     */
    bool isCategory();

    /**
     * @brief Set the currently open tab
     * @param editor
     */
    void setOpenTab(QWidget *editor = Q_NULLPTR);
    QWidget* openTab();

    QJsonObject toJson() const;

    QString host() const;
    void setHost(const QString &host);

    int port() const;
    void setPort(const int &port);

    QString username() const;
    void setUsername(const QString &username);

    QString keyPath() const;
    void setKeyPath(const QString &keyPath);

    QString keyPassphrase() const;
    void setKeyPassphrase(const QString &keyPassphrase);

private:
    QWidget *_editor;

    QString _host;
    int _port;
    QString _username;
    QString _keyPath;
    QString _keyPassphrase;
};

#endif // NOTETREEITEM_HPP
