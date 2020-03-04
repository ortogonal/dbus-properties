#pragma once

#include <QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusConnection>

class DBusAdapterObject : public QObject
{
    Q_OBJECT
public:
    DBusAdapterObject(const QByteArray &service,
                      const QByteArray &path,
                      const QDBusConnection &connection,
                      QObject *parent = nullptr);
protected:
    void connectAdapter(QDBusAbstractAdaptor *obj);

private slots:
    void propertyChanged();

private:
    QDBusConnection m_connection;
    QByteArray m_service;
    QByteArray m_path;
};
