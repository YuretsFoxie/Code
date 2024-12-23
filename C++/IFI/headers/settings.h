#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <string>

class Settings
{
public:
	Settings(const std::string& filePath);
	
	std::string getSerialPort() const;
	int getBaudRate() const;
	int getBatchSize() const;
	int getMaxADCValue() const;
	int getMaxPoints() const;
	float getWindowWidth() const;
	float getWindowHeight() const;
	
private:
	void readConfigFile(const std::string& filePath);
	void parseConfigLine(const std::string& line);
	
	std::string serialPort;
	int baudRate;
	int batchSize;
	int maxADCValue;
	int maxPoints;
	float windowWidth;
	float windowHeight;	
};

#endif // SETTINGS_H_INCLUDED
