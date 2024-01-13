#include "file.h"

void File::create(const string& name, const string& content)
{
	HANDLE file = ::CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD bytesWritten;
	::WriteFile(file, content.c_str(), content.size(), &bytesWritten, NULL);
	::CloseHandle(file);
}



bool File::isExist(const string& path)
{
	DWORD attributes = ::GetFileAttributes(path.c_str());
	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}



bool File::isLater(const FILETIME& first, const FILETIME& second)
{
	if (first.dwHighDateTime > second.dwHighDateTime)
		return true;
	
	if (first.dwHighDateTime == second.dwHighDateTime)
		return first.dwLowDateTime > second.dwLowDateTime;
	
	return false;
}



vector<string> File::findAll(const string& folder)
{
	WIN32_FIND_DATAA findData;
	HANDLE file = ::FindFirstFile((folder + "/*").c_str(), &findData);
	vector<string> files;
	
	while (::FindNextFileA(file, &findData))
	{
		string name = string(findData.cFileName);
		if (name != "..")
			files.push_back(name);
	}
	
	::FindClose(file);
	return files;
}



string File::extractName(const string& fullName)
{
	return fullName.substr(0, fullName.find("."));
}



FILETIME File::getTime(const string& name)
{
	HANDLE file = ::CreateFile(name.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME writeTime;
	::GetFileTime(file, NULL, NULL, &writeTime);
	::CloseHandle(file);
	
	return writeTime;
}
