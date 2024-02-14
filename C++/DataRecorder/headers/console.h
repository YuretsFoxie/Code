#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <iostream>

using namespace std;

class Console
{
public:
	static Console& shared();
	void setFrame();
	
	template<typename T>
	void print(const T& data)
	{
		cout << count << "> " << data << '\n';
		count++;
	}
	
private:
	Console() {}
	~Console() {}
	
	int count = 1;
};

#endif // CONSOLE_H_INCLUDED
