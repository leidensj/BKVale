#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T22:15:42
#
#-------------------------------------------------

QT       += core gui serialport sql network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BaitaAssistente
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tinyexpr.c \
    settingsdlg.cpp \
    printutils.cpp \
    databaseutils.cpp \
    note.cpp \
    settings.cpp \
    reminderview.cpp \
    reminderwidget.cpp \
    reminderdatabase.cpp \
    itemwidget.cpp \
    consumptionview.cpp \
    consumptiondatabase.cpp \
    consumptionwidget.cpp \
    consumptionfilter.cpp \
    qcustomplot.cpp \
    consumptionchart.cpp \
    calculatorwidget.cpp \
    noteview.cpp \
    notewidget.cpp \
    logindialog.cpp \
    jlineedit.cpp \
    user.cpp \
    usermgtwidget.cpp \
    usermgtdatabase.cpp \
    usermgtview.cpp \
    notetablewidget.cpp \
    itemview.cpp \
    jdatabase.cpp \
    categoryview.cpp \
    categorywidget.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    note.h \
    settings.h \
    reminder.h \
    reminderview.h \
    reminderwidget.h \
    reminderdatabase.h \
    item.h \
    itemwidget.h \
    consumption.h \
    consumptionview.h \
    consumptiondatabase.h \
    consumptionwidget.h \
    consumptionfilter.h \
    qcustomplot.h \
    consumptionchart.h \
    calculatorwidget.h \
    calculator.h \
    noteview.h \
    notewidget.h \
    logindialog.h \
    jlineedit.h \
    user.h \
    usermgtwidget.h \
    usermgtdatabase.h \
    usermgtview.h \
    notetablewidget.h \
    itemview.h \
    jdatabase.h \
    category.h \
    categoryview.h \
    categorywidget.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
