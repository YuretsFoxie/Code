#ifndef DATABUFFER_H_INCLUDED
#define DATABUFFER_H_INCLUDED

#include <vector>
#include "settings.h"
#include "spectrumanalyzer.h"

class DataBuffer
{
public:
	DataBuffer(Settings& settings, SpectrumAnalyzer& analyzer);
	
	void push(const char newData);
	std::vector<float>& getData();
	
private:
	SpectrumAnalyzer& analyzer;
	
	const int bufferSize;
	std::vector<float> buffer;
};

#endif // DATABUFFER_H_INCLUDED
