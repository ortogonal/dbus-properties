#include "example.h"

#include <QDebug>

Example::Example(QObject *parent)
    : DBusInterfaceObject<org::example>("/example", QDBusConnection::sessionBus(), parent)
{
    connect(interface(), &org::example::newUSBDevice,
            this, &Example::newUSBDevice);
}

int Example::tick() const
{
    return m_tick;
}

int Example::red() const
{
    return m_red;
}

int Example::green() const
{
    return m_green;
}

int Example::blue() const
{
    return m_blue;
}

void Example::test(int value)
{
    interface()->test(value);
}

void Example::setProperties(const QVariantMap &propertyChanges)
{
    for (auto it = propertyChanges.cbegin(); it != propertyChanges.cend(); ++it) {
        if (it.key() == "tick") {
            m_tick = it.value().toInt();
            emit tickChanged();
        } else if (it.key() == "red") {
            m_red = it.value().toInt();
            emit redChanged();
        } else if (it.key() == "green") {
            m_green = it.value().toInt();
            emit greenChanged();
        } else if (it.key() == "blue") {
            m_blue = it.value().toInt();
            emit blueChanged();
        }
    }
}
