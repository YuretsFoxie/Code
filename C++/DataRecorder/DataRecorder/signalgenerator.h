#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
#include <QTimer>
#include "settings.h"

class SignalGenerator: public QObject
{
public:
    explicit SignalGenerator(QObject *parent = nullptr);
   ~SignalGenerator();

    void toggle();

signals:
    void notifyValue(const int value);

private slots:
    void onTimer();

private:
    void start();
    void stop();
    void generateSeparately();
    void generateSum();

    const double pi = acos(-1);
    const double samplingFrequency = Settings::shared().getData().samplingFrequency;
    const int timeInterval = 1000.0 / samplingFrequency;

    Q_OBJECT
    QTimer* timer;
    QVector<Signal> testSignals = Settings::shared().getSignals();
    double t = 0;
    int currentChannel = 0;
};

#endif // SIGNALGENERATOR_H
