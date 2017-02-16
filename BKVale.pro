#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T22:15:42
#
#-------------------------------------------------

QT       += core gui serialport sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BKVale
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tinyexpr.c \
    settingsdlg.cpp \
    bkframe.cpp \
    printutils.cpp \
    databaseutils.cpp \
    cons.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    bkframe.h \
    printutils.h \
    cons.h \
    databaseutils.h

FORMS    += mainwindow.ui \
    settingsdlg.ui \
    bkframe.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
