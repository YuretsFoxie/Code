#include "profiler.h"

Profiler::Profiler()
{
	::QueryPerformanceFrequency(&frequency);
}

void Profiler::start()
{
	::QueryPerformanceCounter(&t1);
}

void Profiler::stop()
{
	::QueryPerformanceCounter(&t2);
	double elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	cout << elapsedTime << " ms\n";
}
