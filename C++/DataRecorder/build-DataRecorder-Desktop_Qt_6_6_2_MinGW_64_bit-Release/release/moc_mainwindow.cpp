/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataRecorder/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onUpdateConnected",
    "",
    "onUpdateDisconnected",
    "onUpdateMessage",
    "message",
    "onUpdateError",
    "onUpdateValue",
    "value",
    "onFFTReady",
    "QList<double>",
    "result",
    "onSoundCompleted",
    "onConnect",
    "onTest",
    "onClear",
    "onQuit"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[11];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[16];
    char stringdata5[8];
    char stringdata6[14];
    char stringdata7[14];
    char stringdata8[6];
    char stringdata9[11];
    char stringdata10[14];
    char stringdata11[7];
    char stringdata12[17];
    char stringdata13[10];
    char stringdata14[7];
    char stringdata15[8];
    char stringdata16[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 17),  // "onUpdateConnected"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 20),  // "onUpdateDisconnected"
        QT_MOC_LITERAL(51, 15),  // "onUpdateMessage"
        QT_MOC_LITERAL(67, 7),  // "message"
        QT_MOC_LITERAL(75, 13),  // "onUpdateError"
        QT_MOC_LITERAL(89, 13),  // "onUpdateValue"
        QT_MOC_LITERAL(103, 5),  // "value"
        QT_MOC_LITERAL(109, 10),  // "onFFTReady"
        QT_MOC_LITERAL(120, 13),  // "QList<double>"
        QT_MOC_LITERAL(134, 6),  // "result"
        QT_MOC_LITERAL(141, 16),  // "onSoundCompleted"
        QT_MOC_LITERAL(158, 9),  // "onConnect"
        QT_MOC_LITERAL(168, 6),  // "onTest"
        QT_MOC_LITERAL(175, 7),  // "onClear"
        QT_MOC_LITERAL(183, 6)   // "onQuit"
    },
    "MainWindow",
    "onUpdateConnected",
    "",
    "onUpdateDisconnected",
    "onUpdateMessage",
    "message",
    "onUpdateError",
    "onUpdateValue",
    "value",
    "onFFTReady",
    "QList<double>",
    "result",
    "onSoundCompleted",
    "onConnect",
    "onTest",
    "onClear",
    "onQuit"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x0a,    1 /* Public */,
       3,    0,   81,    2, 0x0a,    2 /* Public */,
       4,    1,   82,    2, 0x0a,    3 /* Public */,
       6,    1,   85,    2, 0x0a,    5 /* Public */,
       7,    1,   88,    2, 0x0a,    7 /* Public */,
       9,    1,   91,    2, 0x0a,    9 /* Public */,
      12,    0,   94,    2, 0x08,   11 /* Private */,
      13,    0,   95,    2, 0x08,   12 /* Private */,
      14,    0,   96,    2, 0x08,   13 /* Private */,
      15,    0,   97,    2, 0x08,   14 /* Private */,
      16,    0,   98,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onUpdateConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUpdateDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUpdateMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onUpdateError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onUpdateValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'onFFTReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<double> &, std::false_type>,
        // method 'onSoundCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClear'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onQuit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onUpdateConnected(); break;
        case 1: _t->onUpdateDisconnected(); break;
        case 2: _t->onUpdateMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->onUpdateError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onUpdateValue((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onFFTReady((*reinterpret_cast< std::add_pointer_t<QList<double>>>(_a[1]))); break;
        case 6: _t->onSoundCompleted(); break;
        case 7: _t->onConnect(); break;
        case 8: _t->onTest(); break;
        case 9: _t->onClear(); break;
        case 10: _t->onQuit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
