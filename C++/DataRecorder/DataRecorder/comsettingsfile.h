#ifndef COMSETTINGSFILE_H
#define COMSETTINGSFILE_H

#include "comsettingsdata.h"

class COMSettingsFile
{
public:
    void save(const COMSettingsData& data);
    COMSettingsData load();

private:
    const QString fileName = "comsettings.ini";
    const QString end = "\n";
};

#endif // COMSETTINGSFILE_H
