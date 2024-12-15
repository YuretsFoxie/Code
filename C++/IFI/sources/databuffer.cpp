#include "databuffer.h"

// Public Functins

DataBuffer::DataBuffer(int maxPoints, int scaleFactor): maxPoints(maxPoints), scaleFactor(scaleFactor)
{
	dataBuffer.reserve(maxPoints);
}

void DataBuffer::push(float newData)
{
	int convertedData = convertTwosComplementToInt(std::bitset<8>(newData));
	dataBuffer.push_back(convertedData);
	
	if (dataBuffer.size() > maxPoints)
		dataBuffer.erase(dataBuffer.begin());
}

std::vector<float> DataBuffer::getData()
{
	return dataBuffer;
}

// Private Functions

int DataBuffer::convertTwosComplementToInt(const std::bitset<8>& byte)
{
	return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}
