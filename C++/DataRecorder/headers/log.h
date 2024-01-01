#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <fstream>
#include <string>

using namespace std;

class Log
{
public:
	static Log& shared();
	
	template<typename T>
	void save(const T& data)
	{
		file << getTime() << data << '\n';
	}
	
private:
	Log();
	~Log();
	
	string getTime();
	string getDate();
	
	const string name = "log.txt";
	ofstream file;
};

#endif // LOG_H_INCLUDED
