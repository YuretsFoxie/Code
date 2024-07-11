#include "signalgenerator.h"

// Public Functions

SignalGenerator::SignalGenerator(QObject *parent): QObject{parent}
{
    timer = new QTimer(this);
    timer->setInterval(timeInterval);
    connect(timer, QTimer::timeout, this, onTimer);
}

SignalGenerator::~SignalGenerator()
{
    if (timer->isActive())
        stop();

    delete timer;
}

void SignalGenerator::toggle()
{
    timer->isActive() ? stop() : start();
}

// Private Functions

void SignalGenerator::onTimer()
{
    generateSeparately();
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

void SignalGenerator::generateSeparately()
{
    double signalValue = testSignals[currentChannel].calculate(t);
    emit notifyValue(signalValue);

    currentChannel++;
    if (currentChannel == testSignals.count())
    {
        currentChannel = 0;
        t += 1.0 / samplingFrequency;
    }
}

void SignalGenerator::generateSum()
{
    double signalValue = 0;

    for (Signal s: testSignals)
        signalValue += s.calculate(t);

    emit notifyValue(signalValue);
    t += 1.0 / samplingFrequency;
}
