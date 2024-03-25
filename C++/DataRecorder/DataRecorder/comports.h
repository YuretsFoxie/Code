#ifndef COMPORTS_H
#define COMPORTS_H

#include <QObject>
#include <QSerialPort>

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
};

#endif // COMPORTS_H
