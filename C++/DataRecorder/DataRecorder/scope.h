#ifndef SCOPE_H
#define SCOPE_H

#include <QObject>
#include "qcustomplot.h"
#include "settings.h"

class Scope: public QObject
{
public:
    explicit Scope(QCustomPlot* plot, QObject *parent = nullptr);
    void push(int value);
    void clear();

private:
    int currentChannel = 0;

    Q_OBJECT
    QCustomPlot* plot;
    QVector<QCPGraph*> graphs;
    SettingsData settings = Settings::shared().getData();
};

#endif // SCOPE_H
