#ifndef GRAPHINTERFACE_H_INCLUDED
#define GRAPHINTERFACE_H_INCLUDED

#include <string>

using namespace std;

class GraphInterface
{
public:
	virtual ~GraphInterface() {};
	virtual void addValue(const int value) = 0;
	virtual void print(const int value) = 0;
	virtual void print(const string& message) = 0;
	virtual void render() = 0;
};

#endif // GRAPHINTERFACE_H_INCLUDED
