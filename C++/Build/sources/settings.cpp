#include "settings.h"

Settings::Settings(const string& fileName)
{
	ifstream file(fileName.c_str(), ios::in);
	vector<string> data;
	
	for (string s; getline(file, s); data.push_back(s));
	file.close();
	
	for (string s: data)
	{	
		if (s.find("compiler")  != string::npos)	compiler  = s.substr(s.find(": ") + 2);
		if (s.find("headers")   != string::npos)	headers	  = s.substr(s.find(": ") + 2);
		if (s.find("libraries") != string::npos)	libraries = s.substr(s.find(": ") + 2);
		if (s.find("isConsole") != string::npos)	isConsole = s.substr(s.find(": ") + 2) == "1";	
	}
}
