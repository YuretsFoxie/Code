#ifndef SPECTRUMANALYZER_H_INCLUDED
#define SPECTRUMANALYZER_H_INCLUDED

#include <vector>
#include <math.h>
#include <complex>
#include "settings.h"

typedef std::complex<double> cd;

class SpectrumAnalyzer
{
public:
	SpectrumAnalyzer(Settings& settings);
	
	void push(const std::vector<float>& data);
	std::vector<float>& getData();

private:
	struct SplittedItems
	{
		std::vector<cd> evens;
		std::vector<cd> odds;
	};
	
	SpectrumAnalyzer() {};
   ~SpectrumAnalyzer() {};
	
	SplittedItems split(const std::vector<cd>& v);
	std::vector<cd> calculateFFT(const std::vector<cd>& v);
	
	const double pi = acos(-1);
	const cd i = cd(0.0, 1.0);
	
	std::vector<cd> timeDomainData;
	std::vector<float> frequencyDomainData;
};

#endif // SPECTRUMANALYZER_H_INCLUDED
