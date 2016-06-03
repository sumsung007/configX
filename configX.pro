#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T16:11:19
#
#-------------------------------------------------

QT += core gui widgets

TARGET = configX
TEMPLATE = app
CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/servertree/servertreewidget.cpp \
    src/servertree/servertreeitem.cpp \
    src/configeditor/configeditor.cpp \
    src/servertree/serveritemdialog.cpp \
    src/configeditor/modules/nginxwidget.cpp \
    src/configeditor/modules/basewidget.cpp \
    src/configeditor/sshsession.cpp

HEADERS += \
    src/mainwindow.hpp \
    src/servertree/servertreewidget.hpp \
    src/servertree/servertreeitem.hpp \
    src/configeditor/configeditor.h \
    src/servertree/serveritemdialog.h \
    src/configeditor/modules/nginxwidget.h \
    src/configeditor/modules/basewidget.h \
    src/configeditor/sshsession.h

FORMS += \
    src/mainwindow.ui \
    src/configeditor/configeditor.ui \
    src/servertree/serveritemdialog.ui \
    src/configeditor/modules/nginxwidget.ui

DISTFILES += \
    .gitignore

win32 {
    INCLUDEPATH += "C:/Program Files (x86)/libssh/include"
    LIBS += -L"C:/Program Files (x86)/libssh/lib" -lssh
}

unix {
    INCLUDEPATH += /usr/include/libssh
    LIBS += -lssh
}
