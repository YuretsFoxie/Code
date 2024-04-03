#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include "qcustomplot.h"
#include "settings.h"

class Graph: public QObject
{
public:
    explicit Graph(QCustomPlot* plot, QObject *parent = nullptr);
    void update(int value);
    void clear();

private:
    Q_OBJECT
    QCustomPlot* plot;
    QCPGraph* graph;
    SettingsData settings = Settings::shared().getData();
};

#endif // GRAPH_H
