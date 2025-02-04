#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Vertex {
    double x, y;
};

struct Polyline {
    std::vector<Vertex> vertices;
};

// Function to parse a FlashMX DXF file and extract polylines
std::vector<Polyline> parseDXF(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Polyline> polylines;
    Polyline currentPolyline;
    bool inPolyline = false;
	
    while (std::getline(file, line)) {
		if (line == "0") {
            /*
			if (inPolyline) {
                polylines.push_back(currentPolyline);
                currentPolyline.vertices.clear();
                inPolyline = false;
            }
			*/
            std::getline(file, line);
            if (line == "POLYLINE") {
                inPolyline = true;
            }
        }
		if (inPolyline && line == "VERTEX") {
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

// Function to write G-code header
void writeGCodeHeader(std::ofstream& outFile) {
    outFile << "G21 ; Set units to millimeters\n";
    outFile << "G90 ; Absolute positioning\n";
    outFile << "M3 S10000 ; Spindle on\n";
    outFile << "G1 F50 ; Set feed rate\n";
    outFile << "G0 Z5 ; Raise the tool\n";
}

// Function to write G-code footer
void writeGCodeFooter(std::ofstream& outFile) {
    outFile << "G0 Z5 ; Raise the tool\n";
    outFile << "G0 X0 Y0 ; Move to home position\n";
    outFile << "M5 ; Spindle off\n";
    outFile << "M30 ; End of program\n";
}

// Function to write G-code for polylines
void writePolylineGCode(std::ofstream& outFile, const Polyline& polyline) {
    if (polyline.vertices.empty()) return;

    // Move to the starting point of the polyline
    outFile << "G0 X" << polyline.vertices[0].x << " Y" << polyline.vertices[0].y << "\n";
    outFile << "G1 Z0 ; Lower the tool\n";

    // Draw the polyline
    for (size_t i = 1; i < polyline.vertices.size(); ++i) {
        outFile << "G1 X" << polyline.vertices[i].x << " Y" << polyline.vertices[i].y << "\n";
    }

    // Lift the tool after finishing the polyline
    outFile << "G0 Z5 ; Raise the tool\n";
}

int main() {
    std::string dxfFile = "FlashMX.dxf";
    std::string gcodeFile = "output.tap";

    std::vector<Polyline> polylines = parseDXF(dxfFile);
    std::ofstream outFile(gcodeFile);

    if (!outFile.is_open()) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }

    // Write G-code header
    writeGCodeHeader(outFile);

    // Write G-code for each polyline
    for (const auto& polyline : polylines) {
        writePolylineGCode(outFile, polyline);
    }

    // Write G-code footer
    writeGCodeFooter(outFile);

    outFile.close();
    std::cout << "G-code has been generated: " << gcodeFile << std::endl;
	std::cin.get();
    return 0;
}
