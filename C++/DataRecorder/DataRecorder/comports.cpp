#include <QSerialPortInfo>
#include "comports.h"

// Public Functions

COMPorts::COMPorts(QObject *parent): QObject{parent}, port(nullptr) {}

COMPorts::~COMPorts()
{
    disconnect();
}

void COMPorts::onToggle(const COMSettingsData& data)
{
    port ? disconnect() : connect(data);
}

void COMPorts::onTransmit(const int value)
{
    transmit(value);
}

// Private Functions

void COMPorts::onDataReady()
{
    if (port->isOpen())
    {
        QByteArray buffer = port->readAll();
        QDataStream stream(buffer);
        int value;

        while (!stream.atEnd())
        {
            stream >> value;
            emit notifyValue(value);
        }
    }
}

void COMPorts::connect(const COMSettingsData& data)
{
    disconnect();

    port = new QSerialPort(this);
    port->setPortName( QSerialPortInfo::availablePorts()[data.portIndex].portName() );
    port->setBaudRate( QSerialPortInfo::standardBaudRates()[data.baudrateIndex] );
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);

    if (port->open(QIODevice::ReadWrite))
        QObject::connect(port, QSerialPort::readyRead, this, onDataReady);

    if (port->isOpen())
    {
        transmit(1);
        emit notifyConnected();
    }
    else
        emit notifyError("could not open port");
}

void COMPorts::disconnect()
{
    if (port)
    {
        transmit(0);
        port->close();
        delete port;
        port = nullptr;
        emit notifyDisconnected();
    }
}

void COMPorts::transmit(const int value)
{
    if (!port->isOpen())
        emit notifyError("port is not opened");

    qint64 bytes = port->write(QByteArray().setNum(value));

    if (bytes == -1)
        emit notifyError("transmission failed");
}
