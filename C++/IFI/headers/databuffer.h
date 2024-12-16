#ifndef DATABUFFER_H_INCLUDED
#define DATABUFFER_H_INCLUDED

#include <vector>

class DataBuffer
{
public:
	DataBuffer(int maxPoints);
	
	void push(char newData);
	std::vector<float>& getData();
	
private:
	const int maxPoints;
	std::vector<float> buffer;
};

#endif // DATABUFFER_H_INCLUDED
