#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Settings
{
	Settings() {}
	Settings(const string& fileName);
	
	string compiler;
	string headers;
	string libraries;
	bool isConsole;
};

#endif // SETTINGS_H_INCLUDED
