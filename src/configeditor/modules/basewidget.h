#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QtWidgets/QWidget>

class BaseWidget: public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = 0);
};

#endif // BASEWIDGET_H
