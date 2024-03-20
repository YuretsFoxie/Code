#include "graph.h"
Graph::Graph(QObject *parent): QObject{parent} {}

void Graph::setPlot(QCustomPlot *plot)
{
    this->plot = plot;
    graph = plot->addGraph();
    graph->setPen(QPen(Qt::green, 1));

    plot->setBackground(Qt::transparent);
    plot->xAxis->setBasePen(QPen(Qt::green, 1));
    plot->yAxis->setBasePen(QPen(Qt::green, 1));
    plot->xAxis->setTickPen(QPen(Qt::green, 1));
    plot->yAxis->setTickPen(QPen(Qt::green, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->xAxis->setTickLabelColor(Qt::transparent);
    plot->yAxis->setTickLabelColor(Qt::transparent);

    plot->xAxis->grid()->setPen(QPen(Qt::transparent));
    plot->yAxis->grid()->setPen(QPen(Qt::transparent));
    plot->xAxis->grid()->setZeroLinePen(QPen(Qt::transparent));
    plot->yAxis->grid()->setZeroLinePen(QPen(Qt::transparent));

    plot->rescaleAxes();
    plot->xAxis->setRange(-128, 0);
    plot->yAxis->setRange(-520, 520); // -512 - 511
}

void Graph::onUpdate(int value)
{
    int count = graph->dataCount();
    graph->addData(count, value);
    plot->xAxis->setRange(count + 1, 128, Qt::AlignRight);
    plot->replot();
}
