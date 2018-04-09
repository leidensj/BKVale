#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T22:15:42
#
#-------------------------------------------------

QT       += core gui serialport sql network xml
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
    settings.cpp \
    reminderview.cpp \
    consumptionview.cpp \
    consumptiondatabase.cpp \
    consumptionwidget.cpp \
    consumptionfilter.cpp \
    qcustomplot.cpp \
    consumptionchart.cpp \
    calculatorwidget.cpp \
    noteview.cpp \
    logindialog.cpp \
    jlineedit.cpp \
    user.cpp \
    usermgtwidget.cpp \
    usermgtview.cpp \
    notetablewidget.cpp \
    jdatabase.cpp \
    categoryview.cpp \
    jimageview.cpp \
    imageview.cpp \
    productview.cpp \
    imagetablemodel.cpp \
    personview.cpp \
    personwidget.cpp \
    personpageview.cpp \
    addresspageview.cpp \
    phonepageview.cpp \
    jdatabasepicker.cpp \
    pincodeview.cpp \
    shoppinglistview.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    note.h \
    settings.h \
    reminder.h \
    reminderview.h \
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
    logindialog.h \
    jlineedit.h \
    user.h \
    usermgtwidget.h \
    usermgtview.h \
    notetablewidget.h \
    jdatabase.h \
    category.h \
    categoryview.h \
    jimageview.h \
    image.h \
    imageview.h \
    product.h \
    productview.h \
    jtablecolumn.h \
    imagetablemodel.h \
    person.h \
    address.h \
    personview.h \
    personwidget.h \
    personpageview.h \
    addresspageview.h \
    phonepageview.h \
    phone.h \
    defines.h \
    jitem.h \
    jdatabasepicker.h \
    pincodeview.h \
    shoppinglist.h \
    shoppinglistview.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
