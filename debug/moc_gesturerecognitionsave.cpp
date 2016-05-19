/****************************************************************************
** Meta object code from reading C++ file 'gesturerecognitionsave.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gesturerecognitionsave.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gesturerecognitionsave.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GestureRecognitionSave_t {
    QByteArrayData data[14];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GestureRecognitionSave_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GestureRecognitionSave_t qt_meta_stringdata_GestureRecognitionSave = {
    {
QT_MOC_LITERAL(0, 0, 22), // "GestureRecognitionSave"
QT_MOC_LITERAL(1, 23, 20), // "acquisitionCompleted"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 12), // "frameArrived"
QT_MOC_LITERAL(4, 58, 6), // "saving"
QT_MOC_LITERAL(5, 65, 7), // "opening"
QT_MOC_LITERAL(6, 73, 11), // "handleFrame"
QT_MOC_LITERAL(7, 85, 8), // "startRec"
QT_MOC_LITERAL(8, 94, 7), // "stopRec"
QT_MOC_LITERAL(9, 102, 10), // "chooseFile"
QT_MOC_LITERAL(10, 113, 3), // "fwd"
QT_MOC_LITERAL(11, 117, 3), // "bck"
QT_MOC_LITERAL(12, 121, 16), // "appendAnnotation"
QT_MOC_LITERAL(13, 138, 16) // "createAnnotation"

    },
    "GestureRecognitionSave\0acquisitionCompleted\0"
    "\0frameArrived\0saving\0opening\0handleFrame\0"
    "startRec\0stopRec\0chooseFile\0fwd\0bck\0"
    "appendAnnotation\0createAnnotation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GestureRecognitionSave[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    0,   82,    2, 0x06 /* Public */,
       6,    0,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   84,    2, 0x0a /* Public */,
       8,    0,   85,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x0a /* Public */,
      10,    0,   87,    2, 0x0a /* Public */,
      10,    1,   88,    2, 0x0a /* Public */,
      11,    0,   91,    2, 0x0a /* Public */,
      12,    0,   92,    2, 0x0a /* Public */,
      13,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GestureRecognitionSave::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GestureRecognitionSave *_t = static_cast<GestureRecognitionSave *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->acquisitionCompleted(); break;
        case 1: _t->frameArrived(); break;
        case 2: _t->saving(); break;
        case 3: _t->opening(); break;
        case 4: _t->handleFrame(); break;
        case 5: _t->startRec(); break;
        case 6: _t->stopRec(); break;
        case 7: _t->chooseFile(); break;
        case 8: _t->fwd(); break;
        case 9: _t->fwd((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->bck(); break;
        case 11: _t->appendAnnotation(); break;
        case 12: _t->createAnnotation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GestureRecognitionSave::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GestureRecognitionSave::acquisitionCompleted)) {
                *result = 0;
            }
        }
        {
            typedef void (GestureRecognitionSave::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GestureRecognitionSave::frameArrived)) {
                *result = 1;
            }
        }
        {
            typedef void (GestureRecognitionSave::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GestureRecognitionSave::saving)) {
                *result = 2;
            }
        }
        {
            typedef void (GestureRecognitionSave::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GestureRecognitionSave::opening)) {
                *result = 3;
            }
        }
        {
            typedef void (GestureRecognitionSave::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GestureRecognitionSave::handleFrame)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject GestureRecognitionSave::staticMetaObject = {
    { &GestureRecognition::staticMetaObject, qt_meta_stringdata_GestureRecognitionSave.data,
      qt_meta_data_GestureRecognitionSave,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GestureRecognitionSave::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GestureRecognitionSave::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GestureRecognitionSave.stringdata0))
        return static_cast<void*>(const_cast< GestureRecognitionSave*>(this));
    return GestureRecognition::qt_metacast(_clname);
}

int GestureRecognitionSave::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GestureRecognition::qt_metacall(_c, _id, _a);
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
void GestureRecognitionSave::acquisitionCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void GestureRecognitionSave::frameArrived()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void GestureRecognitionSave::saving()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void GestureRecognitionSave::opening()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void GestureRecognitionSave::handleFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
