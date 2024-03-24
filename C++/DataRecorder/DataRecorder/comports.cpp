#include <QtEndian>
#include "comports.h"

// Public Functions

COMPorts::COMPorts(QObject *parent): QObject{parent}, port(nullptr) {}

COMPorts::~COMPorts()
{
    disconnect();
}

void COMPorts::onToggle()
{
    port ? disconnect() : connect();
}

void COMPorts::onTransmit(const int value)
{
    transmit(value);
}

// Private Functions

void COMPorts::onDataReady()
{
    if (port->isOpen())
        for (auto byte: port->readAll())
        {
            int value = qFromLittleEndian<unsigned char>(byte);
            emit notifyValue(value);
        }
}

void COMPorts::connect()
{
    disconnect();

    COMSettingsData data;
    port = new QSerialPort(this);
    port->setPortName(data.port);
    port->setBaudRate(data.baudrate);
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
