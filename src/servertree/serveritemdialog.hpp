#ifndef SERVERITEMDIALOG_H
#define SERVERITEMDIALOG_H

#include "servertreeitem.hpp"

#include <QtWidgets/QDialog>

namespace Ui {
    class ServerItemDialog;
}

class ServerItemDialog: public QDialog
{
    Q_OBJECT

public:
    explicit ServerItemDialog(ServerTreeItem *item, QWidget *parent = 0);
    ~ServerItemDialog();

private:
    Ui::ServerItemDialog *_ui;
    ServerTreeItem *_item;
};

#endif // SERVERITEMDIALOG_H
