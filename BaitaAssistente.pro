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
    notetablewidget.cpp \
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
    notefilterdlg.cpp \
    discounttablewidget.cpp \
    discountview.cpp \
    jtablewidgetitem.cpp \
    jitemview.cpp \
    jitemtable.cpp \
    phonetablewidget.cpp \
    timecard.cpp \
    storeview.cpp \
    jtimeedit.cpp \
    addresstablewidget.cpp \
    productcodetablewidget.cpp \
    jdateedit.cpp \
    address.cpp \
    category.cpp \
    image.cpp \
    discount.cpp \
    note.cpp \
    phone.cpp \
    product.cpp \
    reminder.cpp \
    reservation.cpp \
    shoppinglist.cpp \
    store.cpp \
    jdatepicker.cpp \
    jplaintextedit.cpp \
    jitemhelper.cpp \
    form.cpp \
    employee.cpp \
    supplier.cpp \
    employeeview.cpp \
    jmodel.cpp \
    activeuser.cpp \
    jitemsql.cpp \
    supplierview.cpp \
    userview.cpp \
    payment.cpp \
    jaddremovebuttons.cpp

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
    notetablewidget.h \
    jdatabase.h \
    category.h \
    categoryview.h \
    jimageview.h \
    image.h \
    imageview.h \
    product.h \
    productview.h \
    address.h \
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
    jtimeedit.h \
    timeinterval.h \
    addresstablewidget.h \
    productcodetablewidget.h \
    jdateedit.h \
    jdatepicker.h \
    jplaintextedit.h \
    jitemhelper.h \
    jitemsql.h \
    form.h \
    employee.h \
    supplier.h \
    employeeview.h \
    jmodel.h \
    activeuser.h \
    supplierview.h \
    userview.h \
    payment.h \
    jaddremovebuttons.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
