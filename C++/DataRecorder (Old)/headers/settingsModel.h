#ifndef SETTINGSMODEL_H_INCLUDED
#define SETTINGSMODEL_H_INCLUDED

#include <vector>

using namespace std;

class SettingsModel
{
public:
    static SettingsModel& shared()
    {
        static SettingsModel s;
        return s;
    }
	
	void save();
	void load();
	void increment();
	void decrement();
	
	void setModeTime();
	void setModeSpeed();
	
	int getTime();
	int getSpeed();
	
private:
	enum SettingsState 
	{
		TIME,
		SPEED
	};
	
	SettingsState state;
	vector<int> data;
	
	SettingsModel(): state(SPEED) {}
};

#endif // SETTINGSMODEL_H_INCLUDED
