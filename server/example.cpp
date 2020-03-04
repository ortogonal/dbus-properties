#include "example.h"

#include <QDebug>
#include <QTimer>

Example::Example(QObject *parent)
    : DBusAdapterObject("org.example", "/example", QDBusConnection::sessionBus(), parent)
{
    m_adapter = new ExampleAdaptor(this);
    connectAdapter(m_adapter);

    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    t->setSingleShot(false);
    t->start();

    connect(t, &QTimer::timeout, this, [&]() { setTick(tick() + 1); });
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

void Example::setTick(const int tick)
{
    if (m_tick != tick) {
        m_tick = tick;
        emit tickChanged();
    }
}

void Example::setRed(const int red)
{
    if (m_red != red) {
        m_red = red;
        emit redChanged();
    }
}

void Example::setGreen(const int green)
{
    if (m_green != green) {
        m_green = green;
        emit greenChanged();
    }
}

void Example::setBlue(const int blue)
{
    if (m_blue != blue) {
        m_blue = blue;
        emit blueChanged();
    }
}
