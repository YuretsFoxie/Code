#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <windows.h>
#include <string>

using namespace std;

class Process
{
public:
	void run(const string& command); 
};

#endif // PROCESS_H_INCLUDED
