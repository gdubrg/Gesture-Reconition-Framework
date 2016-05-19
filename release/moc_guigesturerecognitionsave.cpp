/****************************************************************************
** Meta object code from reading C++ file 'guigesturerecognitionsave.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../guigesturerecognitionsave.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guigesturerecognitionsave.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GUIgesturerecognitionsave_t {
    QByteArrayData data[12];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GUIgesturerecognitionsave_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GUIgesturerecognitionsave_t qt_meta_stringdata_GUIgesturerecognitionsave = {
    {
QT_MOC_LITERAL(0, 0, 25), // "GUIgesturerecognitionsave"
QT_MOC_LITERAL(1, 26, 5), // "start"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "stop"
QT_MOC_LITERAL(4, 38, 15), // "updateInterface"
QT_MOC_LITERAL(5, 54, 12), // "updateSaving"
QT_MOC_LITERAL(6, 67, 13), // "updateOpening"
QT_MOC_LITERAL(7, 81, 8), // "annotate"
QT_MOC_LITERAL(8, 90, 6), // "submit"
QT_MOC_LITERAL(9, 97, 4), // "save"
QT_MOC_LITERAL(10, 102, 10), // "syncSlider"
QT_MOC_LITERAL(11, 113, 5) // "reset"

    },
    "GUIgesturerecognitionsave\0start\0\0stop\0"
    "updateInterface\0updateSaving\0updateOpening\0"
    "annotate\0submit\0save\0syncSlider\0reset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GUIgesturerecognitionsave[] = {

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
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    0,   72,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GUIgesturerecognitionsave::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GUIgesturerecognitionsave *_t = static_cast<GUIgesturerecognitionsave *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start(); break;
        case 1: _t->stop(); break;
        case 2: _t->updateInterface(); break;
        case 3: _t->updateSaving(); break;
        case 4: _t->updateOpening(); break;
        case 5: _t->annotate(); break;
        case 6: _t->submit(); break;
        case 7: _t->save(); break;
        case 8: _t->syncSlider(); break;
        case 9: _t->reset(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GUIgesturerecognitionsave::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GUIgesturerecognitionsave.data,
      qt_meta_data_GUIgesturerecognitionsave,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GUIgesturerecognitionsave::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GUIgesturerecognitionsave::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GUIgesturerecognitionsave.stringdata0))
        return static_cast<void*>(const_cast< GUIgesturerecognitionsave*>(this));
    return QWidget::qt_metacast(_clname);
}

int GUIgesturerecognitionsave::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
