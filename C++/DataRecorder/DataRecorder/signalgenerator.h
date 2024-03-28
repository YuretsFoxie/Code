#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
#include <QTimer>



class Signal
{
public:
    Signal(double amplitude, double frequency, double phaseMultiplier):
        a(amplitude), f(frequency), pm(phaseMultiplier) {}

    double calculate(const double& t)
    {
        return a * sin((2 * pi * f * t) + (pm * pi));
    }

private:
    const double pi = acos(-1);
    const double a;
    const double f;
    const double pm;
};



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

    const double pi = acos(-1);
    const double samplingRate = 128.0;
    const int timeInterval = 1000.0 / samplingRate; // 8, 1000 / 128 = 7.8125

    Q_OBJECT
    QTimer* timer;
    Signal* signal;
    double t = 0;
};

#endif // SIGNALGENERATOR_H
