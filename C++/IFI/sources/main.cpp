#include <windows.h>
#include "settings.h"
#include "shaders.h"
#include "sounds.h"
#include "spectrumanalyzer.h"
#include "databuffer.h"
#include "viewportparameters.h"
#include "plot.h"
#include "spectrum.h"
#include "text.h"
#include "graphics.h"
#include "comport.h"
#include "application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Settings			settings	= Settings();
	Shaders				shaders		= Shaders();
	Sounds				sounds		= Sounds();
	SpectrumAnalyzer	analyzer1	= SpectrumAnalyzer(settings);
	SpectrumAnalyzer	analyzer2	= SpectrumAnalyzer(settings);
	DataBuffer			buffer1		= DataBuffer(settings, analyzer1);
	DataBuffer			buffer2		= DataBuffer(settings, analyzer2);
	Plot				plot1		= Plot(settings, buffer1, ViewPortParameters(settings));
	Plot				plot2		= Plot(settings, buffer2, ViewPortParameters(settings));
	Spectrum			spectrum1	= Spectrum(settings, analyzer1, ViewPortParameters(settings));
	Spectrum			spectrum2	= Spectrum(settings, analyzer2, ViewPortParameters(settings));
	Text				text		= Text(ViewPortParameters(settings));
	Graphics			graphics	= Graphics(settings, shaders, plot1, plot2, spectrum1, spectrum2, text);
	COMPort* 			port		= new COMPort(settings, buffer1, buffer2, text, sounds);
	Application* 		application	= new Application(hInstance, nCmdShow, settings, port, graphics);
	
	application->run();
	
	delete port;
	delete application;
	return EXIT_SUCCESS;
}
