#include "signalgenerator.h"

// Public Functions

SignalGenerator::SignalGenerator(QObject *parent): QObject{parent}
{
    timer = new QTimer(this);
    timer->setInterval(timeInterval);
    connect(timer, QTimer::timeout, this, onTimer);

    signal = new Signal(100, 1, 0);
}

SignalGenerator::~SignalGenerator()
{
    if (timer->isActive())
        stop();

    delete timer;
    delete signal;
}

void SignalGenerator::toggle()
{
    timer->isActive() ? stop() : start();
}

// Private Functions

void SignalGenerator::onTimer()
{
    emit notifyValue(signal->calculate(t));
    t += 1.0 / samplingRate;
}

void SignalGenerator::start()
{
    timer->start();
}

void SignalGenerator::stop()
{
    timer->stop();
    t = 0;
}
