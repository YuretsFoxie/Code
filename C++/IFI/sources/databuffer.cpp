#include "databuffer.h"

// Public Functins

DataBuffer::DataBuffer(int maxPoints, int scaleFactor) : MAX_POINTS(maxPoints), SCALE_FACTOR(scaleFactor)
{
	dataBuffer.reserve(MAX_POINTS);
}

void DataBuffer::push(float newData)
{
	std::lock_guard<std::mutex> lock(bufferMutex);
	dataBuffer.push_back(newData);
	
	if (dataBuffer.size() > MAX_POINTS)
		dataBuffer.erase(dataBuffer.begin());
}

void DataBuffer::prepare(std::vector<float>& vertices)
{
	std::lock_guard<std::mutex> lock(bufferMutex);
	vertices.reserve(dataBuffer.size() * 2);
	preComputeVertices(vertices);
}

// Private Functions

void DataBuffer::preComputeVertices(std::vector<float>& vertices)
{
	const float scaleFactor = 2.0f / MAX_POINTS;
	for (size_t i = 0; i < dataBuffer.size(); ++i)
	{
		float x = (scaleFactor * i) - 1.0f;
		float y = convertTwosComplementToInt(std::bitset<8>(dataBuffer[i])) / static_cast<float>(SCALE_FACTOR);
		vertices.push_back(x);
		vertices.push_back(y);
	}
}

int DataBuffer::convertTwosComplementToInt(const std::bitset<8>& byte)
{
	return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}
