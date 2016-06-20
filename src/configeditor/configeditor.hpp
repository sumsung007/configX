#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include "../servertree/servertreeitem.hpp"
#include "sshsession.hpp"

#include <QtWidgets/QWidget>

namespace Ui {
    class ConfigEditor;
}

class ConfigEditor: public QWidget
{
    Q_OBJECT

public:
    explicit ConfigEditor(ServerTreeItem *item = 0, QWidget *parent = 0);
    ~ConfigEditor();

    ServerTreeItem *item() const;
    SshSession *sshSession();

private:
    Ui::ConfigEditor *_ui;
    ServerTreeItem *_item;
    SshSession *_sshSession;
};

#endif // CONFIGEDITOR_H
