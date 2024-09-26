#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include "testsignal.h"
#include "settings.h"
#include <windows.h>
#include <thread>

using namespace std::chrono_literals;

class Generator
{
public:
    static Generator& shared()
    {
        static Generator instance;
        return instance;
    }
	
	void toggle();
	
private:
	Generator();
   ~Generator();
	
	void start();
	void stop();
	void generate();
	
	const double samplingRate = Settings::shared().getData().samplingFrequency;
    const double timeInterval = 1000.0 / samplingRate;
	
	Signal* signal1;
	Signal* signal2;
	
	std::thread workerThread;
	double t = 0;
	int currentSignal = 0;
	bool isWorking = false;
};

#endif // GENERATOR_H_INCLUDED
