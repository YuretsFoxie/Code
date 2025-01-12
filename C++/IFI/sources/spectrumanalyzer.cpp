#include <algorithm>
#include "spectrumanalyzer.h"

// Public Functions

SpectrumAnalyzer::SpectrumAnalyzer(Settings& settings)
{
	timeDomainData = std::vector<cd>(settings.getBufferSize());
	frequencyDomainData = std::vector<float>(settings.getBufferSize() / 2);
}

void SpectrumAnalyzer::push(const std::vector<float>& data)
{
	std::transform(data.begin(), data.end(), timeDomainData.begin(), [](float value) {
		return cd(static_cast<double>(value), 0.0);
	});
	
	std::vector<cd> fft = calculateFFT(timeDomainData);
	int size = frequencyDomainData.size();
	
	for(int i = 0; i < size; i++)
	{
		frequencyDomainData[i] = abs(fft[i]);	// The first magnitude is a DC offset
	}
}

std::vector<float>& SpectrumAnalyzer::getData()
{
	return frequencyDomainData;
}

// Private Functions

SpectrumAnalyzer::SplittedItems SpectrumAnalyzer::split(const std::vector<cd>& v)
{
	std::vector<cd> evens, odds;
	int count = v.size();
	
	for(int i = 0; i < count; i++)
	{
		(i % 2 ? odds : evens).push_back(v[i]);
	}
	
	return {evens, odds};
}

std::vector<cd> SpectrumAnalyzer::calculateFFT(const std::vector<cd>& v)
{
	int n = v.size();
	
	if (n == 1)
	{
		return v;
	}
	
	SplittedItems items = split(v);
	
	std::vector<cd> evens = calculateFFT(items.evens);
	std::vector<cd> odds = calculateFFT(items.odds);
	std::vector<cd> result(n);
	
	cd w = exp(cd(-2 * pi / n) * i);
	
	for(int j = 0; j < n / 2; j++)
	{
		cd term = pow(w, j) * odds[j];
		result[j] = evens[j] + term;
		result[j + n / 2] = evens[j] - term;
	}
	
	return result;
}
