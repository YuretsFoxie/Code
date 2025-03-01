#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

struct Vertex
{
    double x, y;
};

struct Polyline
{
    std::vector<Vertex> vertices;
};

struct BoundingBox
{
    double minX, minY, maxX, maxY;
};

BoundingBox calculateBoundingBox(const std::vector<Polyline>& polylines)
{
    BoundingBox bbox = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
                        std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()};
    for (const auto& polyline : polylines)
    {
        for (const auto& vertex : polyline.vertices)
        {
            if (vertex.x < bbox.minX) bbox.minX = vertex.x;
            if (vertex.y < bbox.minY) bbox.minY = vertex.y;
            if (vertex.x > bbox.maxX) bbox.maxX = vertex.x;
            if (vertex.y > bbox.maxY) bbox.maxY = vertex.y;
        }
    }
    return bbox;
}

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

void writePolylineGCode(std::ofstream& outFile, const Polyline& polyline, double drillingDepth, double startDrillHeight, double scaleX, double scaleY)
{
    if (polyline.vertices.empty()) return;
    
    outFile << "G0 X" << polyline.vertices[0].x * scaleX << " Y" << polyline.vertices[0].y * scaleY << "\n";
    outFile << "G1 Z" << drillingDepth << " ; Lower the tool\n";
    
    for (size_t i = 1; i < polyline.vertices.size(); ++i)
    {
        outFile << "G1 X" << polyline.vertices[i].x * scaleX << " Y" << polyline.vertices[i].y * scaleY << "\n";
    }
    
    outFile << "G0 Z" << startDrillHeight << " ; Raise the tool\n";
}

int main(int argc, char* argv[])
{
    if (argc != 9)
    {
        std::cout << "Usage: " << argv[0] << " <input file> <output file> <drilling feed> <safe Z> <drilling depth> <start drill height> <output width> <output height>\n";
        return 1;
    }

    std::string dxfFile = argv[1];
    std::string gcodeFile = argv[2];
    double drillingFeed = std::stod(argv[3]);
    double safeZ = std::stod(argv[4]);
    double drillingDepth = std::stod(argv[5]);
    double startDrillHeight = std::stod(argv[6]);
    double outputWidth = std::stod(argv[7]);
    double outputHeight = std::stod(argv[8]);

    std::vector<Polyline> polylines = parseDXF(dxfFile);
    BoundingBox bbox = calculateBoundingBox(polylines);

    double scaleX = outputWidth / (bbox.maxX - bbox.minX);
    double scaleY = outputHeight / (bbox.maxY - bbox.minY);

    std::ofstream outFile(gcodeFile);
    
    if (!outFile.is_open())
    {
        std::cout << "error opening output file" << std::endl;
        return 1;
    }
    
    outFile << "G1 F" << drillingFeed << " ; Set drilling feed rate\n";
    outFile << "G0 Z" << safeZ << " ; Safe Z position\n";
    writeGCodeHeader(outFile, startDrillHeight);
    
    for (const auto& polyline : polylines)
    {
        writePolylineGCode(outFile, polyline, drillingDepth, startDrillHeight, scaleX, scaleY);
    }
    
    writeGCodeFooter(outFile, startDrillHeight);
    
    outFile.close();
    std::cout << "G-code has been generated: " << gcodeFile << std::endl;
    return 0;
}
