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
    QByteArrayData data[6];
    char stringdata0[89];
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
QT_MOC_LITERAL(3, 39, 21), // "enterKeyPressedSignal"
QT_MOC_LITERAL(4, 61, 19), // "filterChangedSignal"
QT_MOC_LITERAL(5, 81, 7) // "toUpper"

    },
    "FilterLineEdit\0downArrowPressedSignal\0"
    "\0enterKeyPressedSignal\0filterChangedSignal\0"
    "toUpper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilterLineEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
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
        case 1: _t->enterKeyPressedSignal(); break;
        case 2: _t->filterChangedSignal(); break;
        case 3: _t->toUpper(); break;
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
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilterLineEdit::enterKeyPressedSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FilterLineEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilterLineEdit::filterChangedSignal)) {
                *result = 2;
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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FilterLineEdit::downArrowPressedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FilterLineEdit::enterKeyPressedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FilterLineEdit::filterChangedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_ProductWidget_t {
    QByteArrayData data[18];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProductWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProductWidget_t qt_meta_stringdata_ProductWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ProductWidget"
QT_MOC_LITERAL(1, 14, 21), // "productSelectedSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "Product"
QT_MOC_LITERAL(4, 45, 7), // "product"
QT_MOC_LITERAL(5, 53, 7), // "refresh"
QT_MOC_LITERAL(6, 61, 14), // "enableControls"
QT_MOC_LITERAL(7, 76, 21), // "removeSelectedProduct"
QT_MOC_LITERAL(8, 98, 4), // "save"
QT_MOC_LITERAL(9, 103, 17), // "bSkipConfirmation"
QT_MOC_LITERAL(10, 121, 7), // "discard"
QT_MOC_LITERAL(11, 129, 6), // "create"
QT_MOC_LITERAL(12, 136, 9), // "setFilter"
QT_MOC_LITERAL(13, 146, 11), // "focusFilter"
QT_MOC_LITERAL(14, 158, 15), // "containsPressed"
QT_MOC_LITERAL(15, 174, 22), // "filterDownArrowPressed"
QT_MOC_LITERAL(16, 197, 21), // "filterEnterKeyPressed"
QT_MOC_LITERAL(17, 219, 15) // "productSelected"

    },
    "ProductWidget\0productSelectedSignal\0"
    "\0Product\0product\0refresh\0enableControls\0"
    "removeSelectedProduct\0save\0bSkipConfirmation\0"
    "discard\0create\0setFilter\0focusFilter\0"
    "containsPressed\0filterDownArrowPressed\0"
    "filterEnterKeyPressed\0productSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProductWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   82,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x0a /* Public */,
       7,    0,   84,    2, 0x0a /* Public */,
       8,    1,   85,    2, 0x0a /* Public */,
      10,    1,   88,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    0,   93,    2, 0x0a /* Public */,
      14,    0,   94,    2, 0x0a /* Public */,
      15,    0,   95,    2, 0x0a /* Public */,
      16,    0,   96,    2, 0x0a /* Public */,
      17,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,
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
        case 0: _t->productSelectedSignal((*reinterpret_cast< const Product(*)>(_a[1]))); break;
        case 1: _t->refresh(); break;
        case 2: _t->enableControls(); break;
        case 3: _t->removeSelectedProduct(); break;
        case 4: _t->save((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->discard((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->create(); break;
        case 7: _t->setFilter(); break;
        case 8: _t->focusFilter(); break;
        case 9: _t->containsPressed(); break;
        case 10: _t->filterDownArrowPressed(); break;
        case 11: _t->filterEnterKeyPressed(); break;
        case 12: _t->productSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ProductWidget::*_t)(const Product & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ProductWidget::productSelectedSignal)) {
                *result = 0;
                return;
            }
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ProductWidget::productSelectedSignal(const Product & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
