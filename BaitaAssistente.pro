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
    logindialog.cpp \
    jlineedit.cpp \
    jdatabase.cpp \
    categoryview.cpp \
    jimageview.cpp \
    imageview.cpp \
    productview.cpp \
    jdatabasepicker.cpp \
    pincodeview.cpp \
    shoppinglistview.cpp \
    shoppinglisttable.cpp \
    jdoublespinbox.cpp \
    shopview.cpp \
    reservationview.cpp \
    jspinbox.cpp \
    packageeditor.cpp \
    discounttablewidget.cpp \
    discountview.cpp \
    jtablewidgetitem.cpp \
    jitemview.cpp \
    phonetablewidget.cpp \
    timecard.cpp \
    storeview.cpp \
    jtimeedit.cpp \
    addresstablewidget.cpp \
    productcodetablewidget.cpp \
    jdateedit.cpp \
    jdatepicker.cpp \
    jplaintextedit.cpp \
    employeeview.cpp \
    jmodel.cpp \
    jitemsql.cpp \
    supplierview.cpp \
    userview.cpp \
    jaddremovebuttons.cpp \
    formwidget.cpp \
    jtable.cpp \
    note/notefilterdlg.cpp \
    note/notetablewidget.cpp \
    note/noteview.cpp \
    items/activeuser.cpp \
    items/address.cpp \
    items/category.cpp \
    items/discount.cpp \
    items/employee.cpp \
    items/form.cpp \
    items/image.cpp \
    items/note.cpp \
    items/phone.cpp \
    items/product.cpp \
    items/reminder.cpp \
    items/reservation.cpp \
    items/shoppinglist.cpp \
    items/store.cpp \
    items/supplier.cpp \
    items/user.cpp \
    items/jitemhelper.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    settings.h \
    reminderview.h \
    qcustomplot.h \
    consumptionchart.h \
    calculatorwidget.h \
    calculator.h \
    logindialog.h \
    jlineedit.h \
    jdatabase.h \
    categoryview.h \
    jimageview.h \
    imageview.h \
    productview.h \
    defines.h \
    jitem.h \
    jdatabasepicker.h \
    pincodeview.h \
    shoppinglistview.h \
    shoppinglisttable.h \
    jdoublespinbox.h \
    shopview.h \
    reservationview.h \
    jspinbox.h \
    packageeditor.h \
    escpos.h \
    filterdlg.h \
    discounttablewidget.h \
    discountview.h \
    jtablewidgetitem.h \
    jitemview.h \
    phonetablewidget.h \
    timecard.h \
    storeview.h \
    jtimeedit.h \
    timeinterval.h \
    addresstablewidget.h \
    productcodetablewidget.h \
    jdateedit.h \
    jdatepicker.h \
    jplaintextedit.h \
    jitemsql.h \
    employeeview.h \
    jmodel.h \
    supplierview.h \
    userview.h \
    jaddremovebuttons.h \
    formwidget.h \
    jtable.h \
    note/noteview.h \
    note/notefilterdlg.h \
    note/notetablewidget.h \
    items/address.h \
    items/activeuser.h \
    items/category.h \
    items/discount.h \
    items/employee.h \
    items/form.h \
    items/image.h \
    items/note.h \
    items/phone.h \
    items/product.h \
    items/reminder.h \
    items/reservation.h \
    items/store.h \
    items/shoppinglist.h \
    items/supplier.h \
    items/user.h \
    items/jitemhelper.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
