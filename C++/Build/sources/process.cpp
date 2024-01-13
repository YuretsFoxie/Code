#include "process.h"

void Process::run(const string& command)
{
	char* argument = const_cast< char* >(command.c_str());
	string directory = ".";
	
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ::ZeroMemory( &pi, sizeof(pi) );
	
	::CreateProcess(NULL, argument, NULL, NULL, FALSE, 0, NULL, directory.c_str(), &si, &pi);
    ::WaitForSingleObject(pi.hProcess, INFINITE);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);
}
