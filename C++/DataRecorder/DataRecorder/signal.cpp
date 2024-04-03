#include "signal.h"

double Signal::calculate(const double t)
{
    return a * sin((2 * pi * f * t) + (pm * pi));
}
