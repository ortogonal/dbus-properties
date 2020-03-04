#include "appliance.h"

Appliance::Appliance()
    : DBusAdapterObject("org.appliance", "/appliance", QDBusConnection::sessionBus())
{
    m_adapter = new ApplianceAdaptor(this);
    connectAdapter(m_adapter);
}

bool Appliance::running() const
{
    return m_running;
}

void Appliance::setRunning(const bool running)
{
    if (m_running != running) {
        m_running = running;
        emit runningChanged();
    }
}

bool Appliance::start()
{
    qDebug() << Q_FUNC_INFO;
    return false;
}
