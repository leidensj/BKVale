/****************************************************************************
** Meta object code from reading C++ file 'consumptionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../consumptionwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'consumptionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConsumptionDoubleSpinBox_t {
    QByteArrayData data[1];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConsumptionDoubleSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConsumptionDoubleSpinBox_t qt_meta_stringdata_ConsumptionDoubleSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 24) // "ConsumptionDoubleSpinBox"

    },
    "ConsumptionDoubleSpinBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConsumptionDoubleSpinBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ConsumptionDoubleSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ConsumptionDoubleSpinBox::staticMetaObject = {
    { &QDoubleSpinBox::staticMetaObject, qt_meta_stringdata_ConsumptionDoubleSpinBox.data,
      qt_meta_data_ConsumptionDoubleSpinBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConsumptionDoubleSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConsumptionDoubleSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConsumptionDoubleSpinBox.stringdata0))
        return static_cast<void*>(const_cast< ConsumptionDoubleSpinBox*>(this));
    return QDoubleSpinBox::qt_metacast(_clname);
}

int ConsumptionDoubleSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDoubleSpinBox::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_ConsumptionWidget_t {
    QByteArrayData data[13];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConsumptionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConsumptionWidget_t qt_meta_stringdata_ConsumptionWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ConsumptionWidget"
QT_MOC_LITERAL(1, 18, 16), // "consumptionSaved"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 11), // "Consumption"
QT_MOC_LITERAL(4, 48, 11), // "consumption"
QT_MOC_LITERAL(5, 60, 6), // "search"
QT_MOC_LITERAL(6, 67, 11), // "dateChanged"
QT_MOC_LITERAL(7, 79, 4), // "date"
QT_MOC_LITERAL(8, 84, 14), // "enableControls"
QT_MOC_LITERAL(9, 99, 12), // "itemSelected"
QT_MOC_LITERAL(10, 112, 4), // "Item"
QT_MOC_LITERAL(11, 117, 4), // "item"
QT_MOC_LITERAL(12, 122, 4) // "save"

    },
    "ConsumptionWidget\0consumptionSaved\0\0"
    "Consumption\0consumption\0search\0"
    "dateChanged\0date\0enableControls\0"
    "itemSelected\0Item\0item\0save"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConsumptionWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,
       9,    1,   52,    2, 0x08 /* Private */,
      12,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void ConsumptionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConsumptionWidget *_t = static_cast<ConsumptionWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->consumptionSaved((*reinterpret_cast< const Consumption(*)>(_a[1]))); break;
        case 1: _t->search(); break;
        case 2: _t->dateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 3: _t->enableControls(); break;
        case 4: _t->itemSelected((*reinterpret_cast< const Item(*)>(_a[1]))); break;
        case 5: _t->save(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConsumptionWidget::*_t)(const Consumption & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConsumptionWidget::consumptionSaved)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ConsumptionWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ConsumptionWidget.data,
      qt_meta_data_ConsumptionWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConsumptionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConsumptionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConsumptionWidget.stringdata0))
        return static_cast<void*>(const_cast< ConsumptionWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int ConsumptionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConsumptionWidget::consumptionSaved(const Consumption & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
