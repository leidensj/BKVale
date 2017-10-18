/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaitaAssistant_t {
    QByteArrayData data[13];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaitaAssistant_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaitaAssistant_t qt_meta_stringdata_BaitaAssistant = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BaitaAssistant"
QT_MOC_LITERAL(1, 15, 10), // "initSignal"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "connect"
QT_MOC_LITERAL(4, 35, 10), // "disconnect"
QT_MOC_LITERAL(5, 46, 12), // "showSettings"
QT_MOC_LITERAL(6, 59, 14), // "enableControls"
QT_MOC_LITERAL(7, 74, 4), // "init"
QT_MOC_LITERAL(8, 79, 8), // "showInfo"
QT_MOC_LITERAL(9, 88, 5), // "print"
QT_MOC_LITERAL(10, 94, 4), // "text"
QT_MOC_LITERAL(11, 99, 15), // "printPartialCut"
QT_MOC_LITERAL(12, 115, 15) // "openItemsDialog"

    },
    "BaitaAssistant\0initSignal\0\0connect\0"
    "disconnect\0showSettings\0enableControls\0"
    "init\0showInfo\0print\0text\0printPartialCut\0"
    "openItemsDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaitaAssistant[] = {

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
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
       9,    1,   77,    2, 0x08 /* Private */,
      11,    0,   80,    2, 0x08 /* Private */,
      12,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BaitaAssistant::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BaitaAssistant *_t = static_cast<BaitaAssistant *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->initSignal(); break;
        case 1: _t->connect(); break;
        case 2: _t->disconnect(); break;
        case 3: _t->showSettings(); break;
        case 4: _t->enableControls(); break;
        case 5: _t->init(); break;
        case 6: _t->showInfo(); break;
        case 7: _t->print(); break;
        case 8: { bool _r = _t->print((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->printPartialCut(); break;
        case 10: _t->openItemsDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BaitaAssistant::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BaitaAssistant::initSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BaitaAssistant::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BaitaAssistant.data,
      qt_meta_data_BaitaAssistant,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BaitaAssistant::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaitaAssistant::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaitaAssistant.stringdata0))
        return static_cast<void*>(const_cast< BaitaAssistant*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BaitaAssistant::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void BaitaAssistant::initSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
