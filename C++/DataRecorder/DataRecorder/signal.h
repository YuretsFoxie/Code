#ifndef SIGNAL_H
#define SIGNAL_H

#include <math.h>

class Signal
{
public:
    Signal(double amplitude, double frequency, double phaseMultiplier):
        a(amplitude), f(frequency), pm(phaseMultiplier) {}

    double calculate(const double t);

private:
    const double pi = acos(-1);
    const double a;
    const double f;
    const double pm;
};

#endif // SIGNAL_H
