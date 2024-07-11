#include <fstream>
#include <sstream>
#include "settings.h"

SettingsData Settings::getData() const
{
    return data;
}

QVector<Signal> Settings::getSignals() const
{
    return testSignals;
}

Settings::Settings()
{
    ifstream file;
    file.open(fileName);
    string line;

    while(getline(file, line))
    {
        vector<string> v = split(line);

        if (v[0] == "comport_name")         data.port = read(v);
        if (v[0] == "baudrate")             data.baudrate = QSerialPort::BaudRate(read(v).toInt());
        if (v[0] == "spectrum_n")           data.spectrumN = read(v).toInt();
        if (v[0] == "sampling_frequency")   data.samplingFrequency = read(v).toInt();
        if (v[0] == "channels_number")      data.channelsNumber = read(v).toInt();

        if (v[0] == "signal")               readSignal(v);
    }
}

vector<string> Settings::split(const string& line)
{
    stringstream stream(line);
    vector<string> tokens;
    string buffer;

    while (stream >> buffer)
        tokens.push_back(buffer);

    return tokens;
}

QString Settings::read(const vector<string> &tokens)
{
    return QString::fromStdString(tokens[1]);
}

void Settings::readSignal(const vector<string> &tokens)
{
    double amplitude = QString::fromStdString(tokens[1]).toDouble();
    double frequency = QString::fromStdString(tokens[2]).toDouble();
    double phaseMultiplier = QString::fromStdString(tokens[3]).toDouble();

    Signal signal = Signal(amplitude, frequency, phaseMultiplier);
    testSignals.append(signal);
}
