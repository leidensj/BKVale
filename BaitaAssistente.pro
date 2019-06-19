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
    categoryview.cpp \
    imageview.cpp \
    productview.cpp \
    pincodeview.cpp \
    shoppinglistview.cpp \
    shoppinglisttable.cpp \
    shopview.cpp \
    reservationview.cpp \
    packageeditor.cpp \
    discounttablewidget.cpp \
    discountview.cpp \
    phonetablewidget.cpp \
    timecard.cpp \
    storeview.cpp \
    addresstablewidget.cpp \
    productcodetablewidget.cpp \
    employeeview.cpp \
    supplierview.cpp \
    userview.cpp \
    formwidget.cpp \
    note/notefilterdlg.cpp \
    note/notetablewidget.cpp \
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
    items/jitemex.cpp \
    widgets/jaddremovebuttons.cpp \
    widgets/jdatabase.cpp \
    widgets/jdatabasepicker.cpp \
    widgets/jdateedit.cpp \
    widgets/jdatepicker.cpp \
    widgets/jdoublespinbox.cpp \
    widgets/jimageview.cpp \
    widgets/jitemview.cpp \
    widgets/jlineedit.cpp \
    widgets/jplaintextedit.cpp \
    widgets/jspinbox.cpp \
    widgets/jtable.cpp \
    widgets/jtablewidgetitem.cpp \
    widgets/jtimeedit.cpp \
    items/jitemsql.cpp \
    items/jmodel.cpp \
    note/purchaseview.cpp

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
    categoryview.h \
    productview.h \
    defines.h \
    pincodeview.h \
    shoppinglistview.h \
    shoppinglisttable.h \
    shopview.h \
    reservationview.h \
    packageeditor.h \
    escpos.h \
    filterdlg.h \
    discounttablewidget.h \
    discountview.h \
    phonetablewidget.h \
    timecard.h \
    storeview.h \
    timeinterval.h \
    addresstablewidget.h \
    productcodetablewidget.h \
    employeeview.h \
    supplierview.h \
    userview.h \
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
    items/jitemex.h \
    widgets/jitemview.h \
    widgets/imageview.h \
    widgets/jaddremovebuttons.h \
    widgets/jdatabasepicker.h \
    widgets/jimageview.h \
    widgets/jdatepicker.h \
    widgets/jdoublespinbox.h \
    widgets/jdateedit.h \
    widgets/formwidget.h \
    widgets/jdatabase.h \
    widgets/jplaintextedit.h \
    widgets/jspinbox.h \
    widgets/jtablewidgetitem.h \
    widgets/jtimeedit.h \
    items/jitem.h \
    items/jitemsql.h \
    widgets/jtable.h \
    widgets/jlineedit.h \
    items/jmodel.h \
    formwidget.h \
    imageview.h \
    note/purchaseview.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
