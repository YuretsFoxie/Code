#ifndef PROFILER_H_INCLUDED
#define PROFILER_H_INCLUDED

#include <iostream>
#include <windows.h>

using namespace std;

class Profiler
{
public:
	Profiler();
	void start();
	void stop();
	
private:
	LARGE_INTEGER frequency, t1, t2;
};

#endif // PROFILER_H_INCLUDED
