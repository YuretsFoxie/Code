#ifndef COMPORTS_H
#define COMPORTS_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class COMPorts : public QObject
{
    Q_OBJECT
public:
    explicit COMPorts(QObject *parent = nullptr);

signals:

};

#endif // COMPORTS_H
