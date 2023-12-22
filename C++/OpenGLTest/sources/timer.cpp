#include "timer.h"

// Public Methods

Timer::Timer(): frequency(0.0), startTime(0)
{
    LARGE_INTEGER integer;

    if(QueryPerformanceFrequency(&integer))
    {
        frequency = double(integer.QuadPart);
        QueryPerformanceCounter(&integer);
        startTime = integer.QuadPart;
    }
}

double Timer::getTime()
{
    LARGE_INTEGER integer;

    QueryPerformanceCounter(&integer);
    return double(integer.QuadPart - startTime) / frequency;
}
