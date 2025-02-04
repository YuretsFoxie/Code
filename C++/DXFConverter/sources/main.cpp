#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Vertex
{
	double x, y;
};

struct Polyline
{
	std::vector<Vertex> vertices;
};

std::vector<Polyline> parseDXF(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	std::vector<Polyline> polylines;
	Polyline currentPolyline;
	bool inPolyline = false;
	
	while (std::getline(file, line))
	{
		if (line == "0")
		{
			std::getline(file, line);
			if (line == "POLYLINE")
			{
				inPolyline = true;
			}
		}
		if (inPolyline && line == "VERTEX")
		{
			Vertex vertex;
			std::getline(file, line); // skip 8 (layer)
			std::getline(file, line); // skip 0
			std::getline(file, line); // skip 10 (x-coordinate flag)
			std::getline(file, line);
			vertex.x = std::stod(line);
			std::getline(file, line); // skip 20 (y-coordinate flag)
			std::getline(file, line);
			vertex.y = std::stod(line);
			currentPolyline.vertices.push_back(vertex);
		}
		if (inPolyline && line == "SEQEND") {
			polylines.push_back(currentPolyline);
			currentPolyline.vertices.clear();
			inPolyline = false;
		}
	}
	
	return polylines;
}

void writeGCodeHeader(std::ofstream& outFile, double startDrillHeight)
{
	outFile << "G21 ; Set units to millimeters\n";
	outFile << "G90 ; Absolute positioning\n";
	outFile << "M3 S10000 ; Spindle on\n";
	outFile << "G1 F50 ; Set feed rate\n";
	outFile << "G0 Z" << startDrillHeight << " ; Raise the tool\n";
}

void writeGCodeFooter(std::ofstream& outFile, double startDrillHeight)
{
	outFile << "G0 Z" << startDrillHeight << " ; Raise the tool\n";
	outFile << "G0 X0 Y0 ; Move to home position\n";
	outFile << "M5 ; Spindle off\n";
	outFile << "M30 ; End of program\n";
}

void writePolylineGCode(std::ofstream& outFile, const Polyline& polyline, double drillingDepth, double startDrillHeight)
{
	if (polyline.vertices.empty()) return;
	
	outFile << "G0 X" << polyline.vertices[0].x << " Y" << polyline.vertices[0].y << "\n";
	outFile << "G1 Z" << drillingDepth << " ; Lower the tool\n";
	
	for (size_t i = 1; i < polyline.vertices.size(); ++i)
	{
		outFile << "G1 X" << polyline.vertices[i].x << " Y" << polyline.vertices[i].y << "\n";
	}
	
	outFile << "G0 Z" << startDrillHeight << " ; Raise the tool\n";
}

int main()
{
	std::string dxfFile = "FlashMX.dxf";
	std::string gcodeFile = "output.tap";
	double drillingDepth = -1.0;
	double startDrillHeight = 5.0;
	
	std::vector<Polyline> polylines = parseDXF(dxfFile);
	std::ofstream outFile(gcodeFile);
	
	if (!outFile.is_open())
	{
		std::cout << "error opening output file" << std::endl;
		return 1;
	}
	
	writeGCodeHeader(outFile, startDrillHeight);
	
	for (const auto& polyline : polylines)
	{
		writePolylineGCode(outFile, polyline, drillingDepth, startDrillHeight);
	}
	
	writeGCodeFooter(outFile, startDrillHeight);
	
	outFile.close();
	std::cout << "G-code has been generated: " << gcodeFile << std::endl;
	std::cin.get();
	return 0;
}
