#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
}

void writeGCodeFooter(std::ofstream& outFile) {
    outFile << "M5\n";  // Spindle stop
    outFile << "M30\n";  // Program end
    outFile << "%\n";
}

void writePolylineGCode(std::ofstream& outFile, const Polyline& polyline) {
    for (size_t i = 0; i < polyline.vertices.size(); ++i) {
        if (i == 0) {
            outFile << "G0 X" << polyline.vertices[i].x << " Y" << polyline.vertices[i].y << "\n";
        } else {
            outFile << "G1 X" << polyline.vertices[i].x << " Y" << polyline.vertices[i].y << "\n";
        }
    }
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
        if (line == "0" && std::getline(inFile, line) && line == "POLYLINE") {
            if (inPolyline) {
                writePolylineGCode(outFile, currentPolyline);
                currentPolyline.vertices.clear();
            }
            inPolyline = true;
        } else if (line == "0" && std::getline(inFile, line) && line == "VERTEX") {
            Vertex vertex;
            std::getline(inFile, line);  // Skip "8" line
            std::getline(inFile, line); vertex.x = std::stod(line);  // Read X (10)
            std::getline(inFile, line);  // Skip "20" line
            std::getline(inFile, line); vertex.y = std::stod(line);  // Read Y (20)
            currentPolyline.vertices.push_back(vertex);
        } else if (line == "0" && std::getline(inFile, line) && line == "SEQEND") {
            writePolylineGCode(outFile, currentPolyline);
            currentPolyline.vertices.clear();
            inPolyline = false;
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
