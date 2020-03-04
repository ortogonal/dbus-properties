#pragma once

#include "dbusinterfaceobject.h"
#include "example_interface.h"

class Example : public DBusInterfaceObject<org::example>
{
    Q_OBJECT
    Q_PROPERTY(int tick READ tick NOTIFY tickChanged)
    Q_PROPERTY(int red READ red NOTIFY redChanged)
    Q_PROPERTY(int green READ green NOTIFY greenChanged)
    Q_PROPERTY(int blue READ blue NOTIFY blueChanged)

public:
    explicit Example(QObject *parent = nullptr);

    int tick() const;
    int red() const;
    int green() const;
    int blue() const;

signals:
    void tickChanged();
    void redChanged();
    void greenChanged();
    void blueChanged();
    void newUSBDevice(const QString &mountPath);

public slots:
    void test(int value);
    void setProperties(const QVariantMap &propertyChanges) override;

private:
    int m_tick = 0;
    int m_red = 0;
    int m_green = 0;
    int m_blue = 0;

};
