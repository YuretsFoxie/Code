#ifndef SCOPE_H
#define SCOPE_H

#include <QObject>
#include "qcustomplot.h"
#include "settings.h"

class Scope: public QObject
{
public:
    explicit Scope(QCustomPlot* plot, QObject *parent = nullptr);
    void update(int value);
    void clear();

private:
    Q_OBJECT
    QCustomPlot* plot;
    QCPGraph* graph;
    SettingsData settings = Settings::shared().getData();
};

#endif // SCOPE_H
