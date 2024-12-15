#ifndef DATABUFFER_H_INCLUDED
#define DATABUFFER_H_INCLUDED

#include <vector>
#include <bitset>

class DataBuffer
{
public:
	DataBuffer(int maxPoints, int scaleFactor);
	
	void push(float newData);
	std::vector<float> getData();
	
private:
	int convertTwosComplementToInt(const std::bitset<8>& byte);
	
	const int maxPoints;
	const int scaleFactor;
	std::vector<float> dataBuffer;
};

#endif // DATABUFFER_H_INCLUDED
