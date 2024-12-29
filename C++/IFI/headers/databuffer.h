#ifndef DATABUFFER_H_INCLUDED
#define DATABUFFER_H_INCLUDED

#include <vector>
#include "settings.h"

class DataBuffer
{
public:
	DataBuffer(const Settings& settings);
	
	void push(char newData);
	std::vector<float>& getData();
	
private:
	const int bufferSize;
	std::vector<float> buffer;
};

#endif // DATABUFFER_H_INCLUDED
