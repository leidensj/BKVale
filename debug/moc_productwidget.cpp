/****************************************************************************
** Meta object code from reading C++ file 'productwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../productwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'productwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FilterLineEdit_t {
    QByteArrayData data[5];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FilterLineEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FilterLineEdit_t qt_meta_stringdata_FilterLineEdit = {
    {
QT_MOC_LITERAL(0, 0, 14), // "FilterLineEdit"
QT_MOC_LITERAL(1, 15, 22), // "downArrowPressedSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 19), // "filterChangedSignal"
QT_MOC_LITERAL(4, 59, 7) // "toUpper"

    },
    "FilterLineEdit\0downArrowPressedSignal\0"
    "\0filterChangedSignal\0toUpper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilterLineEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void FilterLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FilterLineEdit *_t = static_cast<FilterLineEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->downArrowPressedSignal(); break;
        case 1: _t->filterChangedSignal(); break;
        case 2: _t->toUpper(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FilterLineEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilterLineEdit::downArrowPressedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FilterLineEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilterLineEdit::filterChangedSignal)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject FilterLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_FilterLineEdit.data,
      qt_meta_data_FilterLineEdit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FilterLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FilterLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FilterLineEdit.stringdata0))
        return static_cast<void*>(const_cast< FilterLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int FilterLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FilterLineEdit::downArrowPressedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FilterLineEdit::filterChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_ProductWidget_t {
    QByteArrayData data[13];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProductWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProductWidget_t qt_meta_stringdata_ProductWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ProductWidget"
QT_MOC_LITERAL(1, 14, 7), // "refresh"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 14), // "enableControls"
QT_MOC_LITERAL(4, 38, 21), // "removeSelectedProduct"
QT_MOC_LITERAL(5, 60, 4), // "save"
QT_MOC_LITERAL(6, 65, 17), // "bSkipConfirmation"
QT_MOC_LITERAL(7, 83, 7), // "discard"
QT_MOC_LITERAL(8, 91, 6), // "create"
QT_MOC_LITERAL(9, 98, 9), // "setFilter"
QT_MOC_LITERAL(10, 108, 11), // "focusFilter"
QT_MOC_LITERAL(11, 120, 15), // "containsPressed"
QT_MOC_LITERAL(12, 136, 16) // "downArrowPressed"

    },
    "ProductWidget\0refresh\0\0enableControls\0"
    "removeSelectedProduct\0save\0bSkipConfirmation\0"
    "discard\0create\0setFilter\0focusFilter\0"
    "containsPressed\0downArrowPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       8,    0,   73,    2, 0x0a /* Public */,
       9,    0,   74,    2, 0x0a /* Public */,
      10,    0,   75,    2, 0x0a /* Public */,
      11,    0,   76,    2, 0x0a /* Public */,
      12,    0,   77,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProductWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProductWidget *_t = static_cast<ProductWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: _t->enableControls(); break;
        case 2: _t->removeSelectedProduct(); break;
        case 3: _t->save((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->discard((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->create(); break;
        case 6: _t->setFilter(); break;
        case 7: _t->focusFilter(); break;
        case 8: _t->containsPressed(); break;
        case 9: _t->downArrowPressed(); break;
        default: ;
        }
    }
}

const QMetaObject ProductWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ProductWidget.data,
      qt_meta_data_ProductWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ProductWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProductWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProductWidget.stringdata0))
        return static_cast<void*>(const_cast< ProductWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int ProductWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
