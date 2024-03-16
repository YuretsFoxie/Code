#include "testdataprovider.h"

TestDataProvider::TestDataProvider()
{
    timer = new QTimer(this);
    connect(timer, QTimer::timeout, this, onTimer);
    timer->setInterval(8); // 1000 / 128 = 7.8125
}

TestDataProvider::~TestDataProvider()
{
    if (timer->isActive())
        timer->stop();

    delete timer;
}

void TestDataProvider::toggleSlot()
{
    timer->isActive() ? timer->stop() : timer->start();
}

void TestDataProvider::onTimer()
{
    emit updateSignal(sin(n) * 500);
    n += 0.1;
}
