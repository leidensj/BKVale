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
  controls/calculatorwidget.cpp \
  controls/databasecombobox.cpp \
  controls/databasepicker.cpp \
  controls/databaseselector.cpp \
  controls/databaseviewer.cpp \
  controls/imageviewer.cpp \
  controls/logindialog.cpp \
  controls/pincodedialog.cpp \
  controls/shopwidget.cpp \
  controls/timecarddialog.cpp \
  editors/addresseditor.cpp \
  editors/formeditor.cpp \
  editors/packageeditor.cpp \
  filters/jfilter.cpp \
  libraries/qcustomplot.cpp \
  libraries/tinyexpr.c \
        mainwindow.cpp \
  purchasereport.cpp \
  settingsdialog.cpp \
  tables/jtable.cpp \
    printutils.cpp \
    databaseutils.cpp \
    settings.cpp \
    consumptionchart.cpp \
    items/activeuser.cpp \
    items/address.cpp \
    items/category.cpp \
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
  views/jitemview.cpp \
    widgets/jaddremovebuttons.cpp \
  widgets/jclicklabel.cpp \
    widgets/jdateedit.cpp \
    widgets/jdatepicker.cpp \
    widgets/jdoublespinbox.cpp \
    widgets/jlineedit.cpp \
    widgets/jplaintextedit.cpp \
    widgets/jspinbox.cpp \
    widgets/jtimeedit.cpp \
    items/jitemsql.cpp \
    items/purchase.cpp \
    tables/purchasetable.cpp \
    tables/shoppinglisttable.cpp \
    tables/addresstable.cpp \
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
    filters/purchasefilter.cpp \
    items/purchaseelement.cpp \
    models/addressmodel.cpp \
    models/categorymodel.cpp \
    models/employeemodel.cpp \
    models/imagemodel.cpp \
    models/phonemodel.cpp \
    models/productmodel.cpp \
    models/purchasemodel.cpp \
    models/remindermodel.cpp \
    models/shoppinglistmodel.cpp \
    models/storemodel.cpp \
    models/suppliermodel.cpp \
    models/usermodel.cpp \
    models/activeusermodel.cpp \
    models/jmodel.cpp \
    models/productcodemodel.cpp \
    items/paymentelement.cpp \
    common.cpp \
    widgets/jregexpvalidator.cpp \
    tableitems/doubleitem.cpp \
    tableitems/dateitem.cpp \
    tableitems/timeitem.cpp \
    tableitems/textitem.cpp \
    tableitems/packageitem.cpp \
    tableitems/sqlitem.cpp \
    tableitems/addressitem.cpp \
    tables/paymenttable.cpp \
    tables/employeehourstable.cpp \
    tables/storeemployeestable.cpp \
  widgets/pdfgenerator.cpp

HEADERS  += mainwindow.h \
    controls/calculatorwidget.h \
    controls/databasecombobox.h \
    controls/databasepicker.h \
    controls/databaseselector.h \
    controls/databaseviewer.h \
    controls/imageviewer.h \
    controls/logindialog.h \
    controls/pincodedialog.h \
    controls/shopwidget.h \
    controls/timecarddialog.h \
    editors/addresseditor.h \
    editors/formeditor.h \
    editors/packageeditor.h \
    filters/jfilter.h \
    libraries/qcustomplot.h \
    libraries/tinyexpr.h \
    purchasereport.h \
    settingsdialog.h \
    tables/jtable.h \
    printutils.h \
    databaseutils.h \
    settings.h \
    consumptionchart.h \
    defines.h \
    escpos.h \
    timeinterval.h \
    items/address.h \
    items/activeuser.h \
    items/category.h \
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
    views/jitemview.h \
    widgets/jclicklabel.h \
    widgets/jaddremovebuttons.h \
    widgets/jdatepicker.h \
    widgets/jdoublespinbox.h \
    widgets/jdateedit.h \
    widgets/jplaintextedit.h \
    widgets/jspinbox.h \
    widgets/jtimeedit.h \
    items/jitem.h \
    items/jitemsql.h \
    widgets/jlineedit.h \
    items/purchase.h \
    tables/shoppinglisttable.h \
    tables/purchasetable.h \
    tables/addresstable.h \
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
    filters/purchasefilter.h \
    items/jitemelementsql.h \
    items/purchaseelement.h \
    models/addressmodel.h \
    models/categorymodel.h \
    models/employeemodel.h \
    models/imagemodel.h \
    models/phonemodel.h \
    models/productmodel.h \
    models/purchasemodel.h \
    models/remindermodel.h \
    models/shoppinglistmodel.h \
    models/storemodel.h \
    models/suppliermodel.h \
    models/usermodel.h \
    models/activeusermodel.h \
    models/jmodel.h \
    models/productcodemodel.h \
    items/paymentelement.h \
    common.h \
    widgets/jregexpvalidator.h \
    tableitems/doubleitem.h \
    tableitems/jtableitem.h \
    tableitems/dateitem.h \
    tableitems/timeitem.h \
    tableitems/textitem.h \
    tableitems/packageitem.h \
    tableitems/sqlitem.h \
    tableitems/addressitem.h \
    tables/paymenttable.h \
    tables/employeehourstable.h \
    tables/storeemployeestable.h \
    widgets/pdfgenerator.h

FORMS    += mainwindow.ui \
  settingsdialog.ui

RESOURCES += \
    icons.qrc
