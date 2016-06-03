#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include "../servertree/servertreeitem.hpp"
#include "modules/basewidget.h"

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

private:
    Ui::ConfigEditor *_ui;
    ServerTreeItem *_item;
    BaseWidget *_currentModule;
};

#endif // CONFIGEDITOR_H
