#include <fstream>
#include <sstream>
#include "settings.h"

// Public Functions

SettingsData Settings::getData() const
{
    return data;
}

// Private Functions

Settings::Settings()
{
    std::ifstream file;
    file.open(fileName);
    std::string line;

    while(getline(file, line))
    {
        std::vector<std::string> v = split(line);
		
        if (v[0] == "spectrum_n")           data.spectrumN = std::stoi(v[1]);
        if (v[0] == "sampling_frequency")   data.samplingFrequency = std::stoi(v[1]);
    }
}

std::vector<std::string> Settings::split(const std::string& line)
{
    std::stringstream stream(line);
    std::vector<std::string> tokens;
    std::string buffer;

    while (stream >> buffer)
        tokens.push_back(buffer);

    return tokens;
}
