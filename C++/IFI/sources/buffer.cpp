#include "buffer.h"

Buffer::Buffer(const int size): size(size)
{
	array = new float[size];
}
	
Buffer::~Buffer()
{
	delete[] array;
}

void Buffer::push(const float value)
{
	deque.push_back(value);
	
	if (deque.size() > size)
		deque.pop_front();
	
	std::copy(deque.begin(), deque.end(), array);
}

float* Buffer::data()
{
	return array;
}

int Buffer::currentSize()
{
	return deque.size();
}

int Buffer::maxSize()
{
	return size;
}
