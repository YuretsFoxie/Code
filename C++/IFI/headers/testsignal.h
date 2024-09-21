#ifndef TESTSIGNAL_H_INCLUDED
#define TESTSIGNAL_H_INCLUDED

#include <math.h>

class Signal
{
public:
	Signal(): a(0), f(0), pm(0) {}
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

#endif // TESTSIGNAL_H_INCLUDED
