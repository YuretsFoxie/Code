#include <QSerialPortInfo>
#include <QtEndian>
#include "comports.h"

// Public Functions

COMPorts::COMPorts(QObject *parent): QObject{parent}, port(nullptr) {}

COMPorts::~COMPorts()
{
    disconnect();
}

void COMPorts::toggle()
{
    port ? disconnect() : connect();
}

void COMPorts::transmit(const int value)
{
    if (port->isOpen())
    {
        QByteArray array;
        array.setNum(value);

        if (port->write(array) == -1)
            emit notifyError("transmission failed");
    }
    else
        emit notifyError("port is not opened");
}

bool COMPorts::isPortAvailable()
{
    QVector<QString> names;

    for (auto port: QSerialPortInfo::availablePorts())
        names.append(port.portName());

    return names.contains(settings.port);
}

// Private Functions

void COMPorts::onDataReady()
{
    if (port->isOpen())
        for (auto byte: port->readAll())
        {
            int value = qFromLittleEndian<signed char>(byte);
            emit notifyValue(value);
        }
}

void COMPorts::connect()
{
    disconnect();
    setupPort();
    openPort();
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

void COMPorts::setupPort()
{
    port = new QSerialPort(this);
    port->setPortName(settings.port);
    port->setBaudRate(settings.baudrate);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
}

void COMPorts::openPort()
{
    if (port->open(QIODevice::ReadWrite))
    {
        QObject::connect(port, QSerialPort::readyRead, this, onDataReady);
        transmit(1);
        emit notifyConnected();
    }
    else
        emit notifyError("could not open port");
}
