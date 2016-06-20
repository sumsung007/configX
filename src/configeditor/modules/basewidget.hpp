#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QtWidgets/QWidget>

class ConfigEditor;

class BaseWidget: public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(ConfigEditor *editor, QWidget *parent = 0);

protected:
    ConfigEditor *editor;
};

#endif // BASEWIDGET_H
