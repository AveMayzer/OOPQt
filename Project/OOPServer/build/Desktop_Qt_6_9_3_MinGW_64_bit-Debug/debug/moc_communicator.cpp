/****************************************************************************
** Meta object code from reading C++ file 'communicator.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../communicator/communicator.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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
struct qt_meta_tag_ZN13TCommunicatorE_t {};
} // unnamed namespace

template <> constexpr inline auto TCommunicator::qt_create_metaobjectdata<qt_meta_tag_ZN13TCommunicatorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TCommunicator",
        "recieved",
        "",
        "msg",
        "send",
        "recieve"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'recieved'
        QtMocHelpers::SignalData<void(QByteArray)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Slot 'send'
        QtMocHelpers::SlotData<void(QByteArray)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Slot 'recieve'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TCommunicator, qt_meta_tag_ZN13TCommunicatorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TCommunicator::staticMetaObject = { {
    QMetaObject::SuperData::link<QUdpSocket::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCommunicatorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCommunicatorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13TCommunicatorE_t>.metaTypes,
    nullptr
} };

void TCommunicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TCommunicator *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->recieved((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->send((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 2: _t->recieve(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TCommunicator::*)(QByteArray )>(_a, &TCommunicator::recieved, 0))
            return;
    }
}

const QMetaObject *TCommunicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCommunicator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCommunicatorE_t>.strings))
        return static_cast<void*>(this);
    return QUdpSocket::qt_metacast(_clname);
}

int TCommunicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QUdpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TCommunicator::recieved(QByteArray _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
