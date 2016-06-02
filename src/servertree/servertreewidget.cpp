#include "servertreewidget.hpp"

#include <QtCore/QDebug>

ServerTreeWidget::ServerTreeWidget(QWidget *parent):
    QTreeWidget(parent)
{
    qDebug() << "Create ServerTreeWidget instance";

    this->_itemMenu = new QMenu(this);

    QAction *deleteAction = new QAction(tr("Delete"), this->_itemMenu);
    this->_itemMenu->addAction(deleteAction);

    QAction *insertItemAction = new QAction(tr("Create note"), this->_itemMenu);
    this->_itemMenu->addAction(insertItemAction);

    this->_itemMenu->insertSeparator(insertItemAction);

    QAction *insertCategoryAction = new QAction(tr("Create category"), this->_itemMenu);
    this->_itemMenu->addAction(insertCategoryAction);

    this->_treeMenu = new QMenu(this);

    this->_treeMenu->addAction(insertItemAction);
    this->_treeMenu->addAction(insertCategoryAction);

    connect(this, &ServerTreeWidget::customContextMenuRequested, [this](const QPoint pos) {
        qDebug() << "Requested note tree context menu";

        if (this->selectedItems().count() > 0)
            this->_itemMenu->exec(this->viewport()->mapToGlobal(pos));
        else
            this->_treeMenu->exec(this->viewport()->mapFromGlobal(pos));
    });

    connect(deleteAction, &QAction::triggered, [this]() {
        qDebug() << "Delete current item";

        QTreeWidgetItem *item = this->selectedItems().first();
        if (!item)
            return;

        delete item;

        emit saveTree();
    });

    connect(insertItemAction, &QAction::triggered, [this]() {
        qDebug() << "Insert item";

        emit addTreeItem(false);
    });

    connect(insertCategoryAction, &QAction::triggered, [this]() {
        qDebug() << "Insert category";

        emit addTreeItem(true);
    });
}

ServerTreeWidget::~ServerTreeWidget()
{
    qDebug() << "Destroy ServerTreeWidget instance";
}

void ServerTreeWidget::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);

    emit saveTree();
}
