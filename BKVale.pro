#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T22:15:42
#
#-------------------------------------------------

QT       += core gui serialport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BKVale
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tinyexpr.c \
    calendardlg.cpp \
    settingsdlg.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    calendardlg.h \
    settingsdlg.h

FORMS    += mainwindow.ui \
    calendardlg.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
