/****************************************************************************
** Meta object code from reading C++ file 'notedatabasewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../notedatabasewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notedatabasewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NoteDatabaseTableModel_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteDatabaseTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteDatabaseTableModel_t qt_meta_stringdata_NoteDatabaseTableModel = {
    {
QT_MOC_LITERAL(0, 0, 22) // "NoteDatabaseTableModel"

    },
    "NoteDatabaseTableModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteDatabaseTableModel[] = {

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

void NoteDatabaseTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NoteDatabaseTableModel::staticMetaObject = {
    { &QSqlTableModel::staticMetaObject, qt_meta_stringdata_NoteDatabaseTableModel.data,
      qt_meta_data_NoteDatabaseTableModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NoteDatabaseTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteDatabaseTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteDatabaseTableModel.stringdata0))
        return static_cast<void*>(const_cast< NoteDatabaseTableModel*>(this));
    return QSqlTableModel::qt_metacast(_clname);
}

int NoteDatabaseTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_NoteDatabaseWidget_t {
    QByteArrayData data[12];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoteDatabaseWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoteDatabaseWidget_t qt_meta_stringdata_NoteDatabaseWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "NoteDatabaseWidget"
QT_MOC_LITERAL(1, 19, 18), // "noteSelectedSignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 4), // "Note"
QT_MOC_LITERAL(4, 44, 4), // "note"
QT_MOC_LITERAL(5, 49, 17), // "noteRemovedSignal"
QT_MOC_LITERAL(6, 67, 2), // "id"
QT_MOC_LITERAL(7, 70, 7), // "refresh"
QT_MOC_LITERAL(8, 78, 18), // "removeSelectedNote"
QT_MOC_LITERAL(9, 97, 14), // "enableControls"
QT_MOC_LITERAL(10, 112, 12), // "noteSelected"
QT_MOC_LITERAL(11, 125, 3) // "idx"

    },
    "NoteDatabaseWidget\0noteSelectedSignal\0"
    "\0Note\0note\0noteRemovedSignal\0id\0refresh\0"
    "removeSelectedNote\0enableControls\0"
    "noteSelected\0idx"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoteDatabaseWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   55,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x0a /* Public */,
      10,    1,   58,    2, 0x08 /* Private */,
      10,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,   11,
    QMetaType::Void,

       0        // eod
};

void NoteDatabaseWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NoteDatabaseWidget *_t = static_cast<NoteDatabaseWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->noteSelectedSignal((*reinterpret_cast< const Note(*)>(_a[1]))); break;
        case 1: _t->noteRemovedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->refresh(); break;
        case 3: _t->removeSelectedNote(); break;
        case 4: _t->enableControls(); break;
        case 5: _t->noteSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->noteSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NoteDatabaseWidget::*_t)(const Note & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NoteDatabaseWidget::noteSelectedSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (NoteDatabaseWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NoteDatabaseWidget::noteRemovedSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject NoteDatabaseWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_NoteDatabaseWidget.data,
      qt_meta_data_NoteDatabaseWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NoteDatabaseWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoteDatabaseWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NoteDatabaseWidget.stringdata0))
        return static_cast<void*>(const_cast< NoteDatabaseWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int NoteDatabaseWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void NoteDatabaseWidget::noteSelectedSignal(const Note & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NoteDatabaseWidget::noteRemovedSignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
