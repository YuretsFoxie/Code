#include "signalgenerator.h"

// Public Functions

SignalGenerator::SignalGenerator(QObject *parent): QObject{parent}
{
    timer = new QTimer(this);
    timer->setInterval(timeInterval);
    connect(timer, QTimer::timeout, this, onTimer);

    signal1 = new Signal(200, 10, 0);
    signal2 = new Signal(200, 30, 0);
}

SignalGenerator::~SignalGenerator()
{
    if (timer->isActive())
        stop();

    delete timer;
    delete signal1;
    delete signal2;
}

void SignalGenerator::toggle()
{
    timer->isActive() ? stop() : start();
}

// Private Functions

void SignalGenerator::onTimer()
{
    emit notifyValue(signal1->calculate(t) + signal2->calculate(t));
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
