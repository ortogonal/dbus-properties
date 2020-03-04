#pragma once

#include "dbusinterfaceobject.h"
#include "appliance_interface.h"

class Appliance : public DBusInterfaceObject<org::appliance>
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit Appliance(QObject *parent = nullptr);

    bool start();

    Q_INVOKABLE bool running() const;

signals:
    void runningChanged();

public slots:
    void setProperties(const QVariantMap &propertyChanges) override;

private:
    bool m_running = false;
};

