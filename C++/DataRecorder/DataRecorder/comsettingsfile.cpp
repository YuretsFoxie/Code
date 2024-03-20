#include <QFile>
#include <QTextStream>
#include "comsettingsfile.h"

void COMSettingsFile::save(const COMSettingsData& data)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.resize(0);

    QTextStream stream(&file);
    stream << data.portIndex << end << data.baudrateIndex << end;

    file.close();
}

COMSettingsData COMSettingsFile::load()
{
    QFile file(fileName);

    if (!file.exists())
        save({0, 0});

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);

    int portIndex = stream.readLine().toInt();
    int baudrateIndex = stream.readLine().toInt();

    file.close();
    return {portIndex, baudrateIndex};
}
