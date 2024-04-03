#ifndef COMPORTS_H
#define COMPORTS_H

#include <QObject>
#include <QSerialPort>
#include "settings.h"

class COMPorts: public QObject
{
public:
    explicit COMPorts(QObject *parent = nullptr);
   ~COMPorts();

    void toggle();
    void transmit(const int value);
    bool isPortAvailable();

signals:
    void notifyConnected();
    void notifyDisconnected();
    void notifyMessage(const QString& message);
    void notifyError(const QString& message);
    void notifyValue(const int value);

private slots:
    void onDataReady();

private:
    void connect();
    void disconnect();
    void setupPort();
    void openPort();

    Q_OBJECT
    QSerialPort* port;
    SettingsData settings = Settings::shared().getData();
};

#endif // COMPORTS_H
