#ifndef COMPORTS_H
#define COMPORTS_H

#include <QObject>
#include <QSerialPort>
#include "comsettingsdata.h"

class COMPorts: public QObject
{
public:
    explicit COMPorts(QObject *parent = nullptr);
   ~COMPorts();

signals:
    void notifyConnected();
    void notifyDisconnected();
    void notifyMessage(const QString& message);
    void notifyError(const QString& message);
    void notifyValue(const int value);

public slots:
    void onToggle(const COMSettingsData& data);
    void onTransmit(const int value);

private slots:
    void onDataReady();

private:
    void connect(const COMSettingsData& data);
    void disconnect();
    void transmit(const int value);

    Q_OBJECT
    QSerialPort* port;
};

#endif // COMPORTS_H
