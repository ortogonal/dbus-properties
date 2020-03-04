#include "dbusadapterobject.h"

#include <QDebug>
#include <QMetaProperty>
#include <QtDBus/QDBusMessage>

DBusAdapterObject::DBusAdapterObject(const QByteArray &service,
                                     const QByteArray &path,
                                     const QDBusConnection &connection,
                                     QObject *parent)
    : QObject(parent)
      , m_connection(connection)
      , m_service(service)
      , m_path(path)
{
    if (!m_connection.isConnected()) {
        qFatal("Cannot connect to the D-Bus session bus.");
    }

    if (!m_connection.registerObject(path, this)) {
        qFatal("Cannot registerObject.");
    }

    if (!m_connection.registerService(service)) {
        qFatal("Cannot registerService.");
    }
}

void DBusAdapterObject::connectAdapter(QDBusAbstractAdaptor *obj)
{
    const QMetaObject *metaObj = obj->parent()->metaObject();

    if (metaObj) {
        for (int i = 0; i < metaObj->propertyCount(); i++) {
            QMetaProperty property = metaObj->property(i);

            if (QString::fromLatin1(property.name()) == "objectName")
                continue;

            if (property.hasNotifySignal()) {
                QMetaMethod signal = property.notifySignal();
                QMetaMethod updateSlot = metaObject()->method(
                    metaObject()->indexOfSlot("propertyChanged()"));
                connect(obj->parent(), signal, this, updateSlot);
            }
        }
    } else {
        qCritical() << "Cannot setup automatic DBus property handling";
    }
}

void DBusAdapterObject::propertyChanged()
{
    const QMetaObject *metaObj = metaObject();
    QByteArray signalName = metaObj->method(senderSignalIndex()).name();
    if (signalName.endsWith("Changed")) {
        QByteArray propertyName = signalName.replace("Changed", "");
        QDBusMessage signal = QDBusMessage::createSignal(m_path,
                                                         "org.freedesktop.DBus.Properties",
                                                         "PropertiesChanged");
        signal << m_service;
        QVariantMap changedProps;
        changedProps.insert(propertyName, property(propertyName));
        signal << changedProps;
        m_connection.send(signal);
    }
}
