#include <bitset>
#include "databuffer.h"

// Public Functins

DataBuffer::DataBuffer(const Settings& settings): bufferSize(settings.getBufferSize())
{
	buffer.reserve(bufferSize);
}

void DataBuffer::push(char newData)
{
	std::bitset<8> byte = std::bitset<8>(newData);
	int convertedData = byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
	buffer.push_back(convertedData);
	
	if (buffer.size() > bufferSize)
	{
		buffer.erase(buffer.begin());
	}
}

std::vector<float>& DataBuffer::getData()
{
	return buffer;
}
