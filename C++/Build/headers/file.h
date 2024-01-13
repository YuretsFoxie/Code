#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>

using namespace std;

class File
{
public:
	void create(const string& name, const string& content);
	bool isExist(const string& path);
	bool isLater(const FILETIME& first, const FILETIME& second);
	vector<string> findAll(const string& folder = ".");
	string extractName(const string& fullName);
	FILETIME getTime(const string& name);
	
private:
	
};

#endif // FILE_H_INCLUDED
