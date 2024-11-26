#include <fstream>
#include "settings.h"

// Public Functions

Settings::Settings(const std::string& filePath)
{
	readConfigFile(filePath);
}

std::string Settings::getSerialPort() const
{ 
	return serialPort;
}

int Settings::getBaudRate() const
{
	return baudRate;
}

int Settings::getBatchSize() const
{
	return batchSize;
}

int Settings::getScaleFactor() const
{
	return scaleFactor;
}

int Settings::getMaxPoints() const
{
	return maxPoints;
}

// Private Functions

void Settings::readConfigFile(const std::string& filePath)
{
	std::ifstream configFile(filePath);
	std::string line;
	
	while (std::getline(configFile, line))
		parseConfigLine(line);
}

void Settings::parseConfigLine(const std::string& line) {
	auto delimiterPos = line.find(": ");
	auto name = line.substr(0, delimiterPos);
	auto value = line.substr(delimiterPos + 2);
		
	if (name == "serialPort")	serialPort = value;
	if (name == "baudRate")		baudRate = std::stoi(value);
	if (name == "batchSize")	batchSize = std::stoi(value);
	if (name == "scaleFactor")	scaleFactor = std::stoi(value);
	if (name == "maxPoints")	maxPoints = std::stoi(value);
}
