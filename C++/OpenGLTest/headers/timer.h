#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <windows.h>

class Timer
{
public:
    Timer();
    double getTime();

private:
    double frequency;
    __int64 startTime;
};

#endif // TIMER_H_INCLUDED
