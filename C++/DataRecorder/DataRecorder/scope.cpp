#include "scope.h"

Scope::Scope(QCustomPlot *plot, QObject *parent): QObject(parent)
{
    this->plot = plot;

    for (int i = 0; i < settings.channelsNumber; i++)
    {
        graphs.append(plot->addGraph());
        graphs[i]->setPen(QPen(Qt::green, 1));
        graphs[i]->setAntialiased(false);
    }

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

    plot->xAxis->setRange(-settings.spectrumN, 0);
    plot->yAxis->setRange(-140, 140); // -128 - 127
}

void Scope::push(int value)
{
    int count = graphs[currentChannel]->dataCount();
    graphs[currentChannel]->addData(count, value);
    currentChannel++;

    if (currentChannel == settings.channelsNumber)
    {
        currentChannel = 0;
        plot->xAxis->setRange(count + 1, settings.spectrumN, Qt::AlignRight);
    }

    plot->replot();
}

void Scope::clear()
{
    for (int i = 0; i < settings.channelsNumber; i++)
        graphs[i]->data()->clear();

    plot->xAxis->setRange(-settings.spectrumN, 0);
    plot->replot();
}
