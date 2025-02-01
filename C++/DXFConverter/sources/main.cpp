#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

struct Vertex {
    double x, y;
};

struct Polyline {
    std::vector<Vertex> vertices;
};

void writeGCodeHeader(std::ofstream& outFile) {
    outFile << "%\n";
    outFile << "G90\n";  // Absolute positioning
    outFile << "M3 S30000\n";  // Spindle on, speed 30000 RPM
    outFile << "G1 F50\n";  // Feed rate 50 mm/min
    outFile << "G0 Z2\n";  // Move to a safe height
}

void writeGCodeFooter(std::ofstream& outFile) {
    outFile << "G0 Z2\n";  // Move to a safe height
    outFile << "G0 X0 Y0\n";  // Move to the home position
    outFile << "M5\n";  // Spindle stop
    outFile << "M30\n";  // Program end
    outFile << "%\n";
}

void writePolylineGCode(std::ofstream& outFile, const Polyline& polyline) {
    if (polyline.vertices.empty()) return;

    // Move to the starting point of the polyline
    outFile << "G0 X" << polyline.vertices[0].x << " Y" << polyline.vertices[0].y << "\n";
    outFile << "G1 Z0\n";  // Move down to start cutting

    // Draw the polyline
    for (size_t i = 1; i < polyline.vertices.size(); ++i) {
        outFile << "G1 X" << polyline.vertices[i].x << " Y" << polyline.vertices[i].y << "\n";
    }

    // Lift the tool after finishing the polyline
    outFile << "G0 Z2\n";  // Move up to safe height
}

int main() {
    std::ifstream inFile("FlashMX.txt");
    std::ofstream outFile("Converted_GCode.nc");

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line;
    Polyline currentPolyline;
    bool inPolyline = false;

    // Write G-code header
    writeGCodeHeader(outFile);

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string code;
        iss >> code;
        if (code == "0") {
            iss >> code;
            if (code == "POLYLINE") {
                if (inPolyline) {
                    writePolylineGCode(outFile, currentPolyline);
                    currentPolyline.vertices.clear();
                }
                inPolyline = true;
            } else if (code == "VERTEX") {
                Vertex vertex;
                std::getline(inFile, line);  // Skip "8" line
                std::getline(inFile, line); vertex.x = std::stod(line);  // Read X (10)
                std::getline(inFile, line);  // Skip "20" line
                std::getline(inFile, line); vertex.y = std::stod(line);  // Read Y (20)
                currentPolyline.vertices.push_back(vertex);
            } else if (code == "SEQEND" && inPolyline) {
                writePolylineGCode(outFile, currentPolyline);
                currentPolyline.vertices.clear();
                inPolyline = false;
            }
        }
    }

    if (inPolyline) {
        writePolylineGCode(outFile, currentPolyline);
    }

    // Write G-code footer
    writeGCodeFooter(outFile);

    inFile.close();
    outFile.close();

    std::cout << "Conversion complete" << std::endl;
    return 0;
}
