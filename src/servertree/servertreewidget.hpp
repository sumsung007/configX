#ifndef NOTETREEWIDGET_HPP
#define NOTETREEWIDGET_HPP

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QMenu>

#include <QtGui/QDropEvent>

class ServerTreeWidget: public QTreeWidget
{
    Q_OBJECT

public:
    ServerTreeWidget(QWidget *parent = 0);
    ~ServerTreeWidget();

    void dropEvent(QDropEvent *event);

signals:
    void saveTree();
    void addTreeItem(bool isCategory);

private:
    QMenu *_itemMenu;
    QMenu *_treeMenu;
};

#endif // NOTETREEWIDGET_HPP
