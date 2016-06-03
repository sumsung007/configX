#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include "../servertree/servertreeitem.hpp"
#include "sshsession.h"

#include <QtWidgets/QWidget>

namespace Ui {
    class ConfigEditor;
}

class ConfigEditor: public QWidget
{
    Q_OBJECT

public:
    explicit ConfigEditor(ServerTreeItem *item, QWidget *parent = 0);
    ~ConfigEditor();

    ServerTreeItem *item() const;

    bool isSshConnected();

protected:
    bool connectSsh();

private:
    Ui::ConfigEditor *_ui;
    ServerTreeItem *_item;
    SshSession *_sshSession;
};

#endif // CONFIGEDITOR_H
