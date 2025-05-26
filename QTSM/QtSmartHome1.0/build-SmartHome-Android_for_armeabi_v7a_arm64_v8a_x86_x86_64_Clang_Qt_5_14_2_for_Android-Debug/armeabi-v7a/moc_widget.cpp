/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../SmartHome/widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[22];
    char stringdata0[328];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 24), // "on_connectButton_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 14), // "connectSuccess"
QT_MOC_LITERAL(4, 48, 16), // "Datatimeout_slot"
QT_MOC_LITERAL(5, 65, 14), // "receiveMessage"
QT_MOC_LITERAL(6, 80, 3), // "buf"
QT_MOC_LITERAL(7, 84, 14), // "QMqttTopicName"
QT_MOC_LITERAL(8, 99, 5), // "topic"
QT_MOC_LITERAL(9, 105, 20), // "on_ledButton_clicked"
QT_MOC_LITERAL(10, 126, 27), // "on_humiChangeButton_clicked"
QT_MOC_LITERAL(11, 154, 19), // "on_clButton_clicked"
QT_MOC_LITERAL(12, 174, 21), // "on_doorButton_clicked"
QT_MOC_LITERAL(13, 196, 20), // "on_fanButton_clicked"
QT_MOC_LITERAL(14, 217, 20), // "on_jsqButton_clicked"
QT_MOC_LITERAL(15, 238, 25), // "on_modePushButton_clicked"
QT_MOC_LITERAL(16, 264, 9), // "refreshUI"
QT_MOC_LITERAL(17, 274, 11), // "sendCommand"
QT_MOC_LITERAL(18, 286, 15), // "updateModeLabel"
QT_MOC_LITERAL(19, 302, 4), // "text"
QT_MOC_LITERAL(20, 307, 14), // "updateHumiEdit"
QT_MOC_LITERAL(21, 322, 5) // "value"

    },
    "Widget\0on_connectButton_clicked\0\0"
    "connectSuccess\0Datatimeout_slot\0"
    "receiveMessage\0buf\0QMqttTopicName\0"
    "topic\0on_ledButton_clicked\0"
    "on_humiChangeButton_clicked\0"
    "on_clButton_clicked\0on_doorButton_clicked\0"
    "on_fanButton_clicked\0on_jsqButton_clicked\0"
    "on_modePushButton_clicked\0refreshUI\0"
    "sendCommand\0updateModeLabel\0text\0"
    "updateHumiEdit\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    0,   91,    2, 0x08 /* Private */,
       5,    2,   92,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    0,  104,    2, 0x08 /* Private */,
      17,    0,  105,    2, 0x08 /* Private */,
      18,    1,  106,    2, 0x08 /* Private */,
      20,    1,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 7,    6,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::Float,   21,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_connectButton_clicked(); break;
        case 1: _t->connectSuccess(); break;
        case 2: _t->Datatimeout_slot(); break;
        case 3: _t->receiveMessage((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QMqttTopicName(*)>(_a[2]))); break;
        case 4: _t->on_ledButton_clicked(); break;
        case 5: _t->on_humiChangeButton_clicked(); break;
        case 6: _t->on_clButton_clicked(); break;
        case 7: _t->on_doorButton_clicked(); break;
        case 8: _t->on_fanButton_clicked(); break;
        case 9: _t->on_jsqButton_clicked(); break;
        case 10: _t->on_modePushButton_clicked(); break;
        case 11: _t->refreshUI(); break;
        case 12: _t->sendCommand(); break;
        case 13: _t->updateModeLabel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->updateHumiEdit((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttTopicName >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Widget.data,
    qt_meta_data_Widget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
