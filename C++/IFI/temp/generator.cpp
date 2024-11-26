#include "generator.h"
#include "application.h"
#include <chrono>
#include <iostream>

// Public Functions

void Generator::toggle()
{
	isWorking ? stop() : start();
}

// Private Functions

Generator::Generator()
{
	signal1 = new Signal(70, 1, 0);
	signal2 = new Signal(50, 3, 0);
}

Generator::~Generator()
{
	delete signal1;
	delete signal2;
}

void Generator::start()
{
	workerThread = std::thread( &Generator::generate, this );
	isWorking = true;
}

void Generator::stop()
{
	isWorking = false;
	
	if (workerThread.joinable())
		workerThread.join();
}

void Generator::generate()
{
	while (isWorking)
	{
		if (currentSignal == 0)
		{
			Application::shared().onReceived( signal1->calculate(t) );
		}
		else if (currentSignal == 1)
		{
			Application::shared().onReceived( signal2->calculate(t) );
			t += 1.0 / samplingRate;
		}
		
		currentSignal = currentSignal == 1 ? 0 : 1;
		std::this_thread::sleep_for( std::chrono::milliseconds( (int)timeInterval ) );
	}
}
