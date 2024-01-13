#include <windows.h>
#include "comsettings.h"

// Public Functions

COMSettings::COMSettings()
{
	load();
}

vector<int> COMSettings::getBaudrates()
{
	return {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000};
}

vector<string> COMSettings::findPorts()
{
	vector<string> result;
	
	for (int i = 0; i <= 255; i++)
	{
		string comport = "COM" + to_string(i);
		WCHAR targetPath[bufferSize];
		
		if (::QueryDosDevice(comport.c_str(), (LPSTR)targetPath, bufferSize))
			result.push_back(comport);
	}
	
	return result;
}

COMSettingsData COMSettings::getData()
{
	return data;
}

void COMSettings::setData(const COMSettingsData& data)
{
	if (this->data != data)
	{
		this->data = data;
		save();
	}
}

// Private Functions

void COMSettings::save()
{
	ofstream file(name.c_str(), ios::out);
	file << data.port << endl << data.baudrate << endl;
	file.close();
}

void COMSettings::load()
{
	ifstream file(name.c_str(), ios::in);
	
	if (!file)
	{
		vector<string> ports = findPorts();
		vector<int> baudrates = getBaudrates();
		
		if (ports.size() && baudrates.size())
		{
			data = {ports[0], baudrates[0]};
			save();
		}
	}
	
	file >> data.port >> data.baudrate;
	file.close();
}
