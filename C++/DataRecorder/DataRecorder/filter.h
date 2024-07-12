#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include "settings.h"

class Filter: public QObject
{
public:
    explicit Filter(QObject *parent = nullptr);

public slots:
    void onUpdateValue(const int value);

signals:
    void notifyValue(const int value);

private:
    const int valuesNumber = Settings::shared().getData().filteringValuesNumber;
    const int channelsNumber = Settings::shared().getData().channelsNumber;

    Q_OBJECT
    int currentValue = 0;
    int currentChannel = 0;
    QVector<int> results;
};

#endif // FILTER_H
