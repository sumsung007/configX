#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QJsonObject>

#include <QtWidgets/QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool saveServerTree();
    void loadServerTree();

    void addServerTreeItem(bool isCategory);

private:
    Ui::MainWindow *_ui;
    QJsonObject _config;

    QString _getConfigPath();
};

#endif // MAINWINDOW_H
