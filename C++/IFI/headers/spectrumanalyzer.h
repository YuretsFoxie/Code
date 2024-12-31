#ifndef SPECTRUMANALYZER_H_INCLUDED
#define SPECTRUMANALYZER_H_INCLUDED

#include <vector>
#include <deque>
#include <math.h>
#include <complex>

typedef std::complex<double> cd;

class SpectrumAnalyzer
{
public:
	static SpectrumAnalyzer& shared()
	{
		static SpectrumAnalyzer instance;
		return instance;
	}
	
	void push(const int value);
	
private:
	struct SplittedItems
	{
		std::deque<cd> evens;
		std::deque<cd> odds;
	};
	
	SpectrumAnalyzer() {};
   ~SpectrumAnalyzer() {};
	
	SplittedItems split(const std::deque<cd>& v);
	std::deque<cd> calculateFFT(const std::deque<cd>& v);
	
	const int n = 128; // n must be a power of two
	const double pi = acos(-1);
	const cd i = cd(0.0, 1.0);
	
	std::deque<cd> buffer;
};

#endif // SPECTRUMANALYZER_H_INCLUDED
