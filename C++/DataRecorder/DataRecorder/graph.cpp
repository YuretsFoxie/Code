#include "graph.h"

Graph::Graph(QCustomPlot *plot, QObject *parent): QObject(parent)
{
    this->plot = plot;
    graph = plot->addGraph();
    graph->setPen(QPen(Qt::green, 1));
    graph->setAntialiased(false);

    plot->setBackground(Qt::transparent);
    plot->rescaleAxes();

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

    plot->xAxis->setRange(-128, 0);
    plot->yAxis->setRange(-140, 140); // -128 - 127
}

void Graph::update(int value)
{
    int count = graph->dataCount();
    graph->addData(count, value);
    plot->xAxis->setRange(count + 1, 128, Qt::AlignRight);
    plot->replot();
}

void Graph::clear()
{
    graph->data()->clear();
    plot->xAxis->setRange(-128, 0);
    plot->replot();
}
