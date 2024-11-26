#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <vector>
#include <string>

struct SettingsData
{
	int spectrumN = 0;
	int samplingFrequency = 0;
};

	
	
class Settings
{
public:
	static Settings& shared()
	{
		static Settings instance;
		return instance;
	}
	
	SettingsData getData() const;
	
private:	
	Settings();
    ~Settings() {}
	
	std::vector<std::string> split(const std::string& line);
	
	const std::string fileName = "settings.ini";
	SettingsData data;
};

#endif // SETTINGS_H_INCLUDED
