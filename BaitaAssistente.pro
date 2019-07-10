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
    qcustomplot.cpp \
    consumptionchart.cpp \
    calculatorwidget.cpp \
    logindialog.cpp \
    pincodeview.cpp \
    packageeditor.cpp \
    discountview.cpp \
    timecard.cpp \
    formwidget.cpp \
    items/activeuser.cpp \
    items/address.cpp \
    items/category.cpp \
    items/discount.cpp \
    items/employee.cpp \
    items/form.cpp \
    items/image.cpp \
    items/phone.cpp \
    items/product.cpp \
    items/reminder.cpp \
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
    items/purchase.cpp \
    tables/purchasetable.cpp \
    tables/shoppinglisttable.cpp \
    tables/addresstable.cpp \
    tables/discounttable.cpp \
    tables/phonetable.cpp \
    tables/productcodetable.cpp \
    views/purchaseview.cpp \
    views/categoryview.cpp \
    views/employeeview.cpp \
    views/imageview.cpp \
    views/reminderview.cpp \
    views/storeview.cpp \
    views/supplierview.cpp \
    views/shoppinglistview.cpp \
    views/productview.cpp \
    views/userview.cpp \
    shopview.cpp \
    widgets/jfilter.cpp \
    filters/purchasefilter.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    settings.h \
    qcustomplot.h \
    consumptionchart.h \
    calculatorwidget.h \
    calculator.h \
    logindialog.h \
    defines.h \
    pincodeview.h \
    shopview.h \
    packageeditor.h \
    escpos.h \
    discountview.h \
    timecard.h \
    timeinterval.h \
    items/address.h \
    items/activeuser.h \
    items/category.h \
    items/discount.h \
    items/employee.h \
    items/form.h \
    items/image.h \
    items/phone.h \
    items/product.h \
    items/reminder.h \
    items/store.h \
    items/shoppinglist.h \
    items/supplier.h \
    items/user.h \
    items/jitemex.h \
    widgets/jitemview.h \
    widgets/jaddremovebuttons.h \
    widgets/jdatabasepicker.h \
    widgets/jimageview.h \
    widgets/jdatepicker.h \
    widgets/jdoublespinbox.h \
    widgets/jdateedit.h \
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
    items/purchase.h \
    tables/shoppinglisttable.h \
    tables/purchasetable.h \
    tables/addresstable.h \
    tables/discounttable.h \
    tables/phonetable.h \
    tables/productcodetable.h \
    views/purchaseview.h \
    views/categoryview.h \
    views/employeeview.h \
    views/imageview.h \
    views/productview.h \
    views/reminderview.h \
    views/supplierview.h \
    views/storeview.h \
    views/shoppinglistview.h \
    views/userview.h \
    widgets/jfilter.h \
    filters/purchasefilter.h

FORMS    += mainwindow.ui \
    settingsdlg.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
