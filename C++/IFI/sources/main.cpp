#include "application.h"
#include "settings.h"
#include "comport.h"
#include "databuffer.h"
#include "spectrumanalyzer.h"
#include "shaders.h"
#include "viewportparameters.h"
#include "graphics.h"
#include "plot.h"
#include "spectrum.h"
#include "text.h"
#include "sounds.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Settings			settings	= Settings();
	Sounds				sounds		= Sounds();
	SpectrumAnalyzer	analyzer1	= SpectrumAnalyzer();
	SpectrumAnalyzer	analyzer2	= SpectrumAnalyzer();
	DataBuffer			buffer1		= DataBuffer(settings, analyzer1);
	DataBuffer			buffer2		= DataBuffer(settings, analyzer2);
	Shaders				shaders		= Shaders();
	Plot				plot1		= Plot(buffer1);	// Plot calls buffer.getData(), when it is ready to be redrawn
	Plot				plot2		= Plot(buffer2);	// 
	Spectrum			spectrum1	= Spectrum(analyzer1);
	Spectrum			spectrum2	= Spectrum(analyzer2);
	Text				text		= Text();
	Graphics			graphics	= Graphics(shaders, buffer1, buffer2, spectrum1, spectrum2, text);
	Comport				comport		= Comport(settings, buffer1, buffer2, text, sounds);
	Application			application	= Application(hInstance, nCmdShow, settings, comport, graphics);
	
	application.run();
	return EXIT_SUCCESS;
}
