/****************************************************************************
** Meta object code from reading C++ file 'notewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../notewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NoteComboBox_t {
    QByteArrayData data[4];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteComboBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteComboBox_t qt_meta_stringdata_NoteComboBox = {
    {
QT_MOC_LITERAL(0, 0, 12), // "NoteComboBox"
QT_MOC_LITERAL(1, 13, 21), // "supplierEnteredSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 7) // "toUpper"

    },
    "NoteComboBox\0supplierEnteredSignal\0\0"
    "toUpper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteComboBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NoteComboBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NoteComboBox *_t = static_cast<NoteComboBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->supplierEnteredSignal(); break;
        case 1: _t->toUpper(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NoteComboBox::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NoteComboBox::supplierEnteredSignal)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject NoteComboBox::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_NoteComboBox.data,
      qt_meta_data_NoteComboBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NoteComboBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteComboBox.stringdata0))
        return static_cast<void*>(const_cast< NoteComboBox*>(this));
    return QComboBox::qt_metacast(_clname);
}

int NoteComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void NoteComboBox::supplierEnteredSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_NoteTableWidget_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteTableWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteTableWidget_t qt_meta_stringdata_NoteTableWidget = {
    {
QT_MOC_LITERAL(0, 0, 15) // "NoteTableWidget"

    },
    "NoteTableWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteTableWidget[] = {

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

void NoteTableWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NoteTableWidget::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_NoteTableWidget.data,
      qt_meta_data_NoteTableWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NoteTableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteTableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteTableWidget.stringdata0))
        return static_cast<void*>(const_cast< NoteTableWidget*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int NoteTableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_NoteWidget_t {
    QByteArrayData data[21];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteWidget_t qt_meta_stringdata_NoteWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NoteWidget"
QT_MOC_LITERAL(1, 11, 13), // "changedSignal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "updateTable"
QT_MOC_LITERAL(4, 38, 3), // "row"
QT_MOC_LITERAL(5, 42, 6), // "column"
QT_MOC_LITERAL(6, 49, 7), // "changed"
QT_MOC_LITERAL(7, 57, 7), // "addItem"
QT_MOC_LITERAL(8, 65, 10), // "removeItem"
QT_MOC_LITERAL(9, 76, 5), // "clear"
QT_MOC_LITERAL(10, 82, 6), // "create"
QT_MOC_LITERAL(11, 89, 6), // "number"
QT_MOC_LITERAL(12, 96, 9), // "suppliers"
QT_MOC_LITERAL(13, 106, 12), // "descriptions"
QT_MOC_LITERAL(14, 119, 10), // "setEnabled"
QT_MOC_LITERAL(15, 130, 7), // "bEnable"
QT_MOC_LITERAL(16, 138, 15), // "supplierEntered"
QT_MOC_LITERAL(17, 154, 16), // "showNoteDatabase"
QT_MOC_LITERAL(18, 171, 7), // "setNote"
QT_MOC_LITERAL(19, 179, 4), // "Note"
QT_MOC_LITERAL(20, 184, 4) // "note"

    },
    "NoteWidget\0changedSignal\0\0updateTable\0"
    "row\0column\0changed\0addItem\0removeItem\0"
    "clear\0create\0number\0suppliers\0"
    "descriptions\0setEnabled\0bEnable\0"
    "supplierEntered\0showNoteDatabase\0"
    "setNote\0Note\0note"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   70,    2, 0x08 /* Private */,
       6,    0,   75,    2, 0x08 /* Private */,
       7,    0,   76,    2, 0x0a /* Public */,
       8,    0,   77,    2, 0x0a /* Public */,
       9,    0,   78,    2, 0x0a /* Public */,
      10,    3,   79,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x0a /* Public */,
      16,    0,   89,    2, 0x0a /* Public */,
      17,    0,   90,    2, 0x0a /* Public */,
      18,    1,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QStringList, QMetaType::QStringList,   11,   12,   13,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void NoteWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NoteWidget *_t = static_cast<NoteWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changedSignal(); break;
        case 1: _t->updateTable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->changed(); break;
        case 3: _t->addItem(); break;
        case 4: _t->removeItem(); break;
        case 5: _t->clear(); break;
        case 6: _t->create((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 7: _t->setEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->supplierEntered(); break;
        case 9: _t->showNoteDatabase(); break;
        case 10: _t->setNote((*reinterpret_cast< const Note(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NoteWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NoteWidget::changedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NoteWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_NoteWidget.data,
      qt_meta_data_NoteWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NoteWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteWidget.stringdata0))
        return static_cast<void*>(const_cast< NoteWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int NoteWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void NoteWidget::changedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
