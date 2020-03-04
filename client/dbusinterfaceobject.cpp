#include "dbusinterfaceobject.h"

#include <QDebug>
#include <QMetaMethod>
#include <QTimer>
#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

DBusInterfaceObjectPrivate::DBusInterfaceObjectPrivate(const QString &service,
                                                       const QString &path,
                                                       const QDBusConnection &connection,
                                                       QObject *parent)
    : QObject(parent)
    , m_service(service)
    , m_path(path)
    , m_connection(connection)
{
    QTimer::singleShot(0, this, [&]() {
        if (serviceRegistered(m_service)) {
            m_connected = true;
            getAllProperties();
        }
    });
}

bool DBusInterfaceObjectPrivate::connected()
{
    return m_connected;
}

bool DBusInterfaceObjectPrivate::serviceRegistered(const QString &service) const
{
    if (m_connection.isConnected()) {
        auto reply = m_connection.interface()->registeredServiceNames();
        for (const QString &s : reply.value()) {
            if (service == s) {
                return true;
            }
        }
    }
    return false;
}

void DBusInterfaceObjectPrivate::getAllProperties()
{
    QDBusInterface iface(m_service, m_path, "org.freedesktop.DBus.Properties", m_connection);

    if (iface.isValid()) {
        QDBusReply<QVariantMap> reply = iface.call("GetAll", "");
        if (reply.isValid()) {
            updateProperties(reply.value());
        } else {
            qDebug() << Q_FUNC_INFO << "Invalid reply";
        }
    } else {
        qDebug() << Q_FUNC_INFO << "errror";
    }
}

void DBusInterfaceObjectPrivate::updateProperties(const QVariantMap properties)
{
    auto metaObject = parent()->metaObject();

    int slotIndex = -1;
    for (int i = 0; i < metaObject->methodCount(); i++) {
        if (metaObject->method(i).name() == "setProperties") {
            slotIndex = i;
            break;
        }
    }

    if (slotIndex > -1) {
        QMetaMethod method = metaObject->method(slotIndex);
        method.invoke(parent(), Qt::QueuedConnection, Q_ARG(QVariantMap, properties));
    }
}

void DBusInterfaceObjectPrivate::propertiesChanged(const QDBusMessage &msg)
{
    QString interface;
    for (const auto &arg : msg.arguments()) {
        if (arg.userType() == qMetaTypeId<QDBusArgument>()) {
            auto properties = qdbus_cast<QVariantMap>(arg);
            updateProperties(properties);
        } else {
            interface = arg.toString();
        }
    }
}

void DBusInterfaceObjectPrivate::serviceOwnerChanged(const QString &name,
                                                     const QString &oldOwner,
                                                     const QString &newOwner)
{
    if (name == m_service) {
        if (!oldOwner.isEmpty() && newOwner.isEmpty()) {
            qDebug() << "Removed";
            m_connected = false;
        } else if (oldOwner.isEmpty() && !newOwner.isEmpty()) {
            qDebug() << "Conncted";
            m_connected = true;
            getAllProperties();
        }
    }
}
