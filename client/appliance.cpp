#include "appliance.h"

Appliance::Appliance(QObject *parent)
    : DBusInterfaceObject<org::appliance>("/appliance", QDBusConnection::sessionBus(), parent)
{}

bool Appliance::start()
{
    auto reply = interface()->start();
    reply.waitForFinished();
    if (reply.isFinished() && reply.isValid()) {
        return reply.value();
    }
    return false;
}

bool Appliance::running() const
{
    return m_running;
}

void Appliance::setProperties(const QVariantMap &propertyChanges)
{
    for (auto it = propertyChanges.cbegin(); it != propertyChanges.cend(); ++it) {
        if (it.key() == "running") {
            m_running = it.value().toBool();
            emit runningChanged();
        }
    }
}
