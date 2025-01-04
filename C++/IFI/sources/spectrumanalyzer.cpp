#include "spectrumanalyzer.h"

// Public Functions

void SpectrumAnalyzer::push(const int value)
{
    buffer.push_back( cd(value, 0) );

    if (buffer.size() < n)
        return;

    if (buffer.size() > n)
        buffer.pop_front();
	
    std::deque<cd> fft = calculateFFT(buffer);
    std::vector<float> magnitudes(n / 2);
	
	for(int i = 0; i < n / 2; i++)
		magnitudes[i] = abs(fft[i]);
	
	// Application::shared().onFFTCalculated(magnitudes);	// The first magnitude is a DC offset
}

// Private Functions

SpectrumAnalyzer::SplittedItems SpectrumAnalyzer::split(const std::deque<cd>& v)
{
	std::deque<cd> evens, odds;
	int count = v.size();
	
	for(int i = 0; i < count; i++)
		(i % 2 ? odds : evens).push_back(v[i]);
	
	return {evens, odds};
}

std::deque<cd> SpectrumAnalyzer::calculateFFT(const std::deque<cd>& v)
{
	int n = v.size();
	
	if (n == 1)
		return v;

	SplittedItems items = split(v);

	std::deque<cd> evens = calculateFFT(items.evens);
	std::deque<cd> odds = calculateFFT(items.odds);
	std::deque<cd> result(n);
	
	cd w = exp(cd(-2 * pi / n) * i);
	
	for(int j = 0; j < n / 2; j++)
	{
		cd term = pow(w, j) * odds[j];
		result[j] = evens[j] + term;
		result[j + n / 2] = evens[j] - term;
	}
	
	return result;
}
