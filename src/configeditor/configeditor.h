#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include "../servertree/servertreeitem.hpp"

#include <QtWidgets/QWidget>

namespace Ui {
    class ConfigEditor;
}

class ConfigEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigEditor(ServerTreeItem *item, QWidget *parent = 0);
    ~ConfigEditor();

private:
    Ui::ConfigEditor *_ui;
    ServerTreeItem *_item;
};

#endif // CONFIGEDITOR_H
