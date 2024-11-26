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
	int getScaleFactor() const;
	int getMaxPoints() const;
	
private:
	void readConfigFile(const std::string& filePath);
	void parseConfigLine(const std::string& line);
	
	std::string serialPort;
	int baudRate;
	int batchSize;
	int scaleFactor;
	int maxPoints;
};

#endif // SETTINGS_H_INCLUDED
