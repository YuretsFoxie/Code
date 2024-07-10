#include "spectrum.h"

Spectrum::Spectrum(QCustomPlot *plot, QObject* parent): QObject(parent)
{
    this->plot = plot;
    plot->setBackground(Qt::transparent);

    plot->xAxis->grid()->setPen(QPen(Qt::transparent));
    plot->yAxis->grid()->setPen(QPen(Qt::transparent));
    plot->xAxis->grid()->setZeroLinePen(QPen(Qt::transparent));
    plot->yAxis->grid()->setZeroLinePen(QPen(Qt::transparent));

    plot->xAxis->setBasePen(QPen(Qt::green, 1));
    plot->yAxis->setBasePen(QPen(Qt::green, 1));
    plot->xAxis->setTickPen(QPen(Qt::green, 1));
    plot->yAxis->setTickPen(QPen(Qt::green, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->xAxis->setTickLabelColor(Qt::transparent);
    plot->yAxis->setTickLabelColor(Qt::transparent);
    plot->xAxis->setRange(0, width);

    bars = new QCPBars(plot->xAxis, plot->yAxis);
    bars->setWidth(0.8);
    bars->setPen(Qt::NoPen);
    bars->setBrush(Qt::green);

    for (int i = 0; i < width; i++)
        x.append(i);

    clear();
}

Spectrum::~Spectrum()
{
    plot->clearPlottables();
}

void Spectrum::update(const QVector<double>& values)
{
    count++;
    if (count < width)
        return;

    count = 0;

    bars->setData(x, values);
    auto max = std::max_element(std::begin(values), std::end(values));
    plot->yAxis->setRange(0, *max);
    plot->replot();
}

void Spectrum::clear()
{
    bars->setData(x, clearY);
    plot->yAxis->setRange(0, 100);
    plot->replot();
}
