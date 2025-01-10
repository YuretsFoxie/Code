#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <string>

class Settings
{
public:
	Settings();
	
	std::string portName() const;
	int getBaudRate() const;
	int getBatchSize() const;
	int getMaxADCValue() const;
	int getBufferSize() const;
	float getWindowWidth() const;
	float getWindowHeight() const;
	
private:
	void readConfigFile(const std::string& filePath);
	void parseConfigLine(const std::string& line);
	
	const std::string& filePath = "settings.ini";
	
	std::string portName;
	int baudRate;
	int batchSize;
	int maxADCValue;
	int bufferSize;
	float windowWidth;
	float windowHeight;	
};

#endif // SETTINGS_H_INCLUDED
