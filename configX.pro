QT += core gui widgets

TARGET = configX
TEMPLATE = app
CONFIG += c++11

SRC_PREFIX = src
include(files.pri)

SOURCES += \
    $${SRC_PREFIX}/main.cpp \
    $${SRC_PREFIX}/mainwindow.cpp \
    $${SRC_PREFIX}/servertree/serveritemdialog.cpp \
    $${SRC_PREFIX}/servertree/servertreewidget.cpp \
    $${SRC_PREFIX}/servertree/servertreeitem.cpp \
    $${SRC_PREFIX}/configeditor/configeditor.cpp \
    $${SRC_PREFIX}/configeditor/sshsession.cpp \
    $${SRC_PREFIX}/configeditor/modules/basewidget.cpp \
    $${SRC_PREFIX}/configeditor/modules/nginxwidget/nginxwidget.cpp

HEADERS += \
    $${SRC_PREFIX}/mainwindow.hpp \
    $${SRC_PREFIX}/servertree/serveritemdialog.hpp \
    $${SRC_PREFIX}/servertree/servertreewidget.hpp \
    $${SRC_PREFIX}/servertree/servertreeitem.hpp \
    $${SRC_PREFIX}/configeditor/configeditor.hpp \
    $${SRC_PREFIX}/configeditor/sshsession.hpp \
    $${SRC_PREFIX}/configeditor/modules/basewidget.hpp \
    $${SRC_PREFIX}/configeditor/modules/nginxwidget/nginxwidget.hpp

FORMS += \
    $${SRC_PREFIX}/mainwindow.ui \
    $${SRC_PREFIX}/configeditor/configeditor.ui \
    $${SRC_PREFIX}/servertree/serveritemdialog.ui \
    $${SRC_PREFIX}/configeditor/modules/nginxwidget/nginxwidget.ui

DISTFILES += \
    .gitignore \
    files.pri

win32 {
    INCLUDEPATH += "C:/Program Files (x86)/libssh/include"
    LIBS += -L"C:/Program Files (x86)/libssh/lib" -lssh
}

unix {
    INCLUDEPATH += /usr/include/libssh
    LIBS += -lssh
}
