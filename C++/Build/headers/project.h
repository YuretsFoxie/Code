#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED

#include <windows.h>
#include <algorithm>
#include <iostream>

#include "settings.h"
#include "process.h"
#include "file.h"

using namespace std;

class Project
{
public:
	bool build();
	
private:
	string findProjectName();
	string buildLinkCommand(const string& name);
	
	bool performInitialCheck();
	bool compileIfNeeded();
	bool compile(const string& name);
	bool link();
	bool isProject(const vector<string>& files);
	bool showError(const string& message);
	
	void createTemplate();
	void creareSourcesForHeaders();
	void deleteObjectsWithoutSources();
	
	const string buildSettings = "build_settings.ini";
	
	Settings settings;
	Process process;
	File file;
	string projectName;
};

#endif // PROJECT_H_INCLUDED
