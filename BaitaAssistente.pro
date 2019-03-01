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
    qcustomplot.cpp \
    consumptionchart.cpp \
    calculatorwidget.cpp \
    noteview.cpp \
    logindialog.cpp \
    jlineedit.cpp \
    user.cpp \
    usermgtview.cpp \
    notetablewidget.cpp \
    jdatabase.cpp \
    categoryview.cpp \
    jimageview.cpp \
    imageview.cpp \
    productview.cpp \
    personview.cpp \
    jdatabasepicker.cpp \
    pincodeview.cpp \
    shoppinglistview.cpp \
    shoppinglisttable.cpp \
    jdoublespinbox.cpp \
    shopview.cpp \
    reservationview.cpp \
    jspinbox.cpp \
    packageeditor.cpp \
    notefilterdlg.cpp \
    discounttablewidget.cpp \
    discountview.cpp \
    jtablewidgetitem.cpp \
    jitemview.cpp \
    jitemtable.cpp \
    phonetablewidget.cpp \
    timecard.cpp \
    storeview.cpp \
    storeemployeetablewidget.cpp \
    timeintervaldlg.cpp \
    jtimeedit.cpp \
    addresstablewidget.cpp \
    productcodetablewidget.cpp \
    jdateedit.cpp \
    address.cpp \
    category.cpp \
    image.cpp \
    discount.cpp \
    note.cpp \
    person.cpp \
    phone.cpp \
    product.cpp \
    reminder.cpp \
    reservation.cpp \
    shoppinglist.cpp \
    store.cpp \
    jdatepicker.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    note.h \
    settings.h \
    reminder.h \
    reminderview.h \
    qcustomplot.h \
    consumptionchart.h \
    calculatorwidget.h \
    calculator.h \
    noteview.h \
    logindialog.h \
    jlineedit.h \
    user.h \
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
    person.h \
    address.h \
    personview.h \
    phone.h \
    defines.h \
    jitem.h \
    jdatabasepicker.h \
    pincodeview.h \
    shoppinglist.h \
    shoppinglistview.h \
    shoppinglisttable.h \
    jdoublespinbox.h \
    shopview.h \
    reservation.h \
    reservationview.h \
    jspinbox.h \
    packageeditor.h \
    escpos.h \
    notefilterdlg.h \
    filterdlg.h \
    discount.h \
    discounttablewidget.h \
    discountview.h \
    jtablewidgetitem.h \
    jitemview.h \
    jitemtable.h \
    phonetablewidget.h \
    timecard.h \
    store.h \
    storeview.h \
    storeemployeetablewidget.h \
    timeintervaldlg.h \
    jtimeedit.h \
    timeinterval.h \
    addresstablewidget.h \
    productcodetablewidget.h \
    jdateedit.h \
    jdatepicker.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
