#ifndef GRAPH_H_BUFFER
#define GRAPH_H_BUFFER

#include <deque>
#include <algorithm>

class Buffer
{
public:
	Buffer(const int size);
   ~Buffer();
	
	void push(const float value);
	float* data();
	int currentSize();
	int maxSize();
	
private:
	const int size;
	std::deque<float> deque;
	float* array;	
};

#endif // GRAPH_H_BUFFER
