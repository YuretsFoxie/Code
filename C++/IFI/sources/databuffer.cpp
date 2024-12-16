#include "databuffer.h"

// Public Functins

DataBuffer::DataBuffer(int maxPoints): maxPoints(maxPoints)
{
	dataBuffer.reserve(maxPoints);
}

void DataBuffer::push(char newData)
{
	std::bitset<8> byte = std::bitset<8>(newData);
	int convertedData = convertTwosComplementToInt(byte);
	dataBuffer.push_back(convertedData);
	
	if (dataBuffer.size() > maxPoints)
		dataBuffer.erase(dataBuffer.begin());
}

std::vector<float>& DataBuffer::getData()
{
	return dataBuffer;
}

// Private Functions

int DataBuffer::convertTwosComplementToInt(const std::bitset<8>& byte)
{
	return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}



/*
#include <bitset>
#include "databuffer.h"

// Public Functins

DataBuffer::DataBuffer(int maxPoints): maxPoints(maxPoints)
{
	buffer.reserve(maxPoints);
}

void DataBuffer::push(char newData)
{
	std::bitset<8> byte = std::bitset<8>(newData);
	float convertedData = byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
	buffer.push_back(convertedData);
	
	if (buffer.size() > maxPoints)
		buffer.erase(buffer.begin());
}

std::vector<float>& DataBuffer::getData()
{
	return buffer;
}
*/
