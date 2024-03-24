#ifndef COMSETTINGSDATA_H
#define COMSETTINGSDATA_H

#include <QSerialPortInfo>

struct COMSettingsData
{
    const QString port = "COM3";
    const QSerialPort::BaudRate baudrate = QSerialPort::Baud57600;
};

#endif // COMSETTINGSDATA_H
