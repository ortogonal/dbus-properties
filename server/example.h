#pragma once

#include "dbusadapterobject.h"
#include "example_adaptor.h"


class Example : public DBusAdapterObject
{
    Q_OBJECT
    Q_PROPERTY(int tick READ tick WRITE setTick NOTIFY tickChanged)
    Q_PROPERTY(int red READ red WRITE setRed NOTIFY redChanged)
    Q_PROPERTY(int green READ green WRITE setGreen NOTIFY greenChanged)
    Q_PROPERTY(int blue READ blue WRITE setBlue NOTIFY blueChanged)

public:
    explicit Example(QObject *parent = nullptr);

    int tick() const;

    int red() const;
    int green() const;
    int blue() const;

public slots:
    void setTick(const int tick);
    void setRed(const int red);
    void setGreen(const int green);
    void setBlue(const int blue);

signals:
    void tickChanged();
    void redChanged();
    void greenChanged();
    void blueChanged();

private:
    ExampleAdaptor *m_adapter = nullptr;
    int m_tick = 0;

    int m_red = 0;
    int m_green = 0;
    int m_blue = 0;

};
