#pragma once

#include "dbusadapterobject.h"
#include "appliance_adaptor.h"

class Appliance : DBusAdapterObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

public:
    Appliance();

    bool running() const;
public slots:
    void setRunning(const bool running);

    bool start();

signals:
    void runningChanged();

private:
    ApplianceAdaptor *m_adapter = nullptr;
    bool m_running = false;
};

