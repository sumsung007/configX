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
    src/servertree/serveritemdialog.cpp

HEADERS += \
    src/mainwindow.hpp \
    src/servertree/servertreewidget.hpp \
    src/servertree/servertreeitem.hpp \
    src/configeditor/configeditor.h \
    src/servertree/serveritemdialog.h

FORMS += \
    src/mainwindow.ui \
    src/configeditor/configeditor.ui \
    src/servertree/serveritemdialog.ui

DISTFILES += \
    .gitignore

win32 {
    INCLUDEPATH += "C:/Program Files (x86)/libssh/include"
    LIBS += -L"C:/Program Files (x86)/libssh/lib" -lssh
}
