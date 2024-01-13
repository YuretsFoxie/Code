#include "project.h"

bool Project::build()
{
	if (!performInitialCheck())
		return showError("not a project folder");
	
	settings = Settings(buildSettings);
	projectName = findProjectName();
	
	creareSourcesForHeaders();
	deleteObjectsWithoutSources();
	
	if (!compileIfNeeded())
		return showError("compilation failed");
	
	if (!link())
		return showError("linking failed");
	
	process.run(projectName);
	return true;
}



string Project::findProjectName()
{
	WIN32_FIND_DATA data;
	::FindClose(::FindFirstFile(".", &data));
	
	return string(data.cFileName) + ".exe";
}



string Project::buildLinkCommand(const string& name)
{
	string parameters = settings.compiler + " ";
	
	if (!settings.isConsole)
		parameters += " -mwindows";
	
	parameters += " -Wall -O2 -o " + name;
	
	if (settings.libraries != "0")
		parameters += " -L .";
	
	vector<string> objects = file.findAll("objects");
	for (string object: objects)
		parameters += " objects/" + object;
	
	if (settings.libraries != "0")
		parameters += " " + settings.libraries;
	
	return parameters;
}



bool Project::performInitialCheck()
{
	if (file.findAll().size() == 1)
		createTemplate();
	
	return isProject(file.findAll());
}



bool Project::compileIfNeeded()
{
	vector<string> sources = file.findAll("sources");
	for (string source: sources)
	{
		string name = file.extractName(source);
		string header = "headers/" + name + ".h";
		string object = "objects/" + name + ".o";
		
		FILETIME objectTime = file.getTime(object);
			
		bool isObject = file.isExist(object);
		bool isSourceChanged = file.isLater(file.getTime(source), objectTime);
		bool isHeaderChanged = ( file.isExist(header) && file.isLater(file.getTime(header), objectTime));
		
		if (!isObject || isSourceChanged || isHeaderChanged)
			if (!compile(name))
				return false;
	}
	
	return true;
}



bool Project::compile(const string& name)
{
	cout << "compiling " + name + ".cpp...\n";
	
	string object = "objects/" + name + ".o";
	string source = "sources/" + name + ".cpp";
	
	if (file.isExist(object))
		::DeleteFile(object.c_str());
	
	string headers = (settings.headers == "0") ? "" : settings.headers;
	string command = settings.compiler + " -I headers/ -I " + headers + " -o " + object + " -c " + source;
	
	process.run(command);
	return file.isExist(object);
}



bool Project::link()
{
	cout << "linking...\n";
	
	if (file.isExist(projectName))
		::DeleteFile(projectName.c_str());
	
	process.run(buildLinkCommand(file.extractName(projectName)));	
	return file.isExist(projectName);
}



bool Project::isProject(const vector<string>& files)
{
	return ::find(files.begin(), files.end(), buildSettings) != files.end() &&
		   ::find(files.begin(), files.end(), "headers") != files.end() &&
		   ::find(files.begin(), files.end(), "sources") != files.end() &&
		   ::find(files.begin(), files.end(), "objects") != files.end();
}



bool Project::showError(const string& message)
{
	cout << "error: " << message;
	cin.get();
	return false;
}



void Project::createTemplate()
{
	::CreateDirectoryA("headers", NULL);
	::CreateDirectoryA("objects", NULL);
	::CreateDirectoryA("sources", NULL);
	
	file.create(buildSettings, "compiler: C:/TDM-GCC-64/bin/g++.exe\nheaders: C:/Data/Libs/Eigen/\nlibraries: 0\nisConsole: 1\n");
	file.create("headers/name.h", "#ifndef NAME_H_INCLUDED\n#define NAME_H_INCLUDED\n\nclass Name\n{\npublic:\n\t\nprivate:\n\t\n};\n\n#endif // NAME_H_INCLUDED\n");
	file.create("sources/main.cpp", "#include <iostream>\n\nusing namespace std;\n\nint main()\n{\n\tcout << \"test\" << endl;\n\t\n\tcin.get();\n\treturn 0;\n}\n");
}



void Project::creareSourcesForHeaders()
{
	vector<string> headers = file.findAll("headers");
	for (string header: headers)
	{
		string source = "sources/" + file.extractName(header) + ".cpp";
		if (!file.isExist(source))
			file.create(source, "#include \"" + header + "\"\n\n");
	}
}



void Project::deleteObjectsWithoutSources()
{
	vector<string> objects = file.findAll("objects");
	for (string object: objects)
	{
		string source = "sources/" + file.extractName(object) + ".cpp";
		if (!file.isExist(source))
			::DeleteFile(("objects/" + object).c_str());
	}
}
