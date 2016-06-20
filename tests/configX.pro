QT += testlib
QT -= gui

TARGET = tst_nginxwidgettest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SRC_PREFIX = ../src
include(../files.pri)

HEADERS += \
    mocksession.hpp

SOURCES += \
    tst_nginxwidgettest.cpp \
    mocksession.cpp

FORMS += \
    src/configeditor/configeditor.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"
