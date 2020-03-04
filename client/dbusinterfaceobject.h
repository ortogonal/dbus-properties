#pragma once

#include <memory>
#include <QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>

class DBusInterfaceObjectPrivate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
    DBusInterfaceObjectPrivate(const QString &service,
                               const QString &path,
                               const QDBusConnection &connection,
                               QObject *parent);
    bool connected();

    bool serviceRegistered(const QString &service) const;
    void getAllProperties();
    void updateProperties(const QVariantMap properties);

public slots:
    void propertiesChanged(const QDBusMessage &msg);
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

signals:
    void propertyChanges(const QVariantMap &propertyChanges);
    void connectedChanged();

public:
    QString m_service;
    QString m_path;
    bool m_connected = false;
    QDBusConnection m_connection;
};

template<typename T>
class DBusInterfaceObject : public QObject
{
public:
    explicit DBusInterfaceObject(const QString &path,
                                 const QDBusConnection &connection,
                                 QObject *parent = nullptr)
        : QObject(parent)
        , m_priv(std::make_unique<DBusInterfaceObjectPrivate>(T::staticInterfaceName(),
                                                              path,
                                                              connection,
                                                              this))
    {
        m_interface = std::make_unique<T>(T::staticInterfaceName(), path, connection);
        m_priv->m_connection.connect(T::staticInterfaceName(),
                                     path,
                                     "org.freedesktop.DBus.Properties",
                                     "PropertiesChanged",
                                     m_priv.get(),
                                     SLOT(propertiesChanged(QDBusMessage)));

        connect(m_priv->m_connection.interface(),
                &QDBusConnectionInterface::serviceOwnerChanged,
                m_priv.get(),
                &DBusInterfaceObjectPrivate::serviceOwnerChanged);
    }

    T *interface() const { return m_interface.get(); }

    bool connected() const { return m_priv->connected(); }

protected:
    virtual void setProperties(const QVariantMap &propertyChanges) = 0;

private:
    std::unique_ptr<DBusInterfaceObjectPrivate> m_priv;
    std::unique_ptr<T> m_interface;
};
