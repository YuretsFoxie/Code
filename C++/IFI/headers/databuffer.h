#ifndef DATABUFFER_H_INCLUDED
#define DATABUFFER_H_INCLUDED

#include <vector>
#include <mutex>
#include <bitset>

class DataBuffer
{
public:
	DataBuffer(int maxPoints, int scaleFactor);
	
	void push(float newData);
	void prepare(std::vector<float>& vertices);
	
private:
	void preComputeVertices(std::vector<float>& vertices);
	int convertTwosComplementToInt(const std::bitset<8>& byte);
	
	const int MAX_POINTS;
	const int SCALE_FACTOR;
	std::vector<float> dataBuffer;
	std::mutex bufferMutex;
};

#endif // DATABUFFER_H_INCLUDED
