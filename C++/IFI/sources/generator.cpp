#include "generator.h"
#include "application.h"

void Generator::generateTestValue()
{
	Application::shared().onReceived(testValue++);
}
