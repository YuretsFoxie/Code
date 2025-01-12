#ifndef SPECTRUM_H_INCLUDED
#define SPECTRUM_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "settings.h"
#include "spectrumanalyzer.h"
#include "viewportparameters.h"

struct Spectrum
{
public:
	Spectrum(Settings& settings, SpectrumAnalyzer& analyzer, const ViewPortParameters& parameters);
   ~Spectrum() {}
	
	void draw() {}
	
private:
	const ViewPortParameters& parameters;
	SpectrumAnalyzer& analyzer;
};

#endif // SPECTRUM_H_INCLUDED
