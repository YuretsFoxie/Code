#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include "qcustomplot.h"

class Graph: public QObject
{
public:
    explicit Graph(QObject *parent = nullptr);
    void setPlot(QCustomPlot* plot);
    void clear();

public slots:
    void onUpdate(int value);

private:
    Q_OBJECT
    QCustomPlot* plot;
    QCPGraph* graph;
};

#endif // GRAPH_H
