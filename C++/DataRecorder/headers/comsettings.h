#ifndef COMSETTINGS_H_INCLUDED
#define COMSETTINGS_H_INCLUDED

#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct COMSettingsData
{
	bool operator == (const COMSettingsData& d)
	{
		return port == d.port && baudrate == d.baudrate;
	}
	
	bool operator != (const COMSettingsData& d)
	{
		return port != d.port || baudrate != d.baudrate;
	}
	
	string port;
	int baudrate;
};



class COMSettings
{
public:
	COMSettings();
	
	vector<int> getBaudrates();
	vector<string> findPorts();	
	COMSettingsData getData();
	void setData(const COMSettingsData& data);
	
private:
	void save();
	void load();
	
	const string name = "comsettings.txt";
	const int bufferSize = 64;
	
	COMSettingsData data;
};

#endif // COMSETTINGS_H_INCLUDED
