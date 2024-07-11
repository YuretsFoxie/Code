#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include <QSerialPort>
#include "signal.h"

using namespace std;

struct SettingsData
{
    QString port = "";
    QSerialPort::BaudRate baudrate = QSerialPort::Baud1200;
    int spectrumN = 0;
    int samplingFrequency = 0;
    int channelsNumber = 0;
};



class Settings
{
public:
    static Settings& shared()
    {
        static Settings instance;
        return instance;
    }

    SettingsData getData() const;
    QVector<Signal> getSignals() const;

private:
    Settings();
    vector<string> split(const string& line);
    QString read(const vector<string>& tokens);
    void readSignal(const vector<string>& tokens);

    const string fileName = "settings.ini";
    SettingsData data;
    QVector<Signal> testSignals;
};

#endif // SETTINGS_H
