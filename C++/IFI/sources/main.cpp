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
	Comport				comport		= Comport(settings);
	DataBuffer			buffer1		= DataBuffer();
	DataBuffer			buffer2		= DataBuffer();
	SpectrumAnalyzer	analyzer1	= SpectrumAnalyzer();
	SpectrumAnalyzer	analyzer2	= SpectrumAnalyzer();
	Shaders				shaders		= Shaders();
	Graphics			graphics	= Graphics();
	Plot				plot1		= Plot();
	Plot				plot2		= Plot();
	Spectrum			spectrum1	= Spectrum();
	Spectrum			spectrum2	= Spectrum();
	Text				text		= Text();
	Sounds				sounds		= Sounds();
	Application			application	= Application(hInstance, nCmdShow, settings, comport, graphics);
	
	application.run();
	return EXIT_SUCCESS;
}
