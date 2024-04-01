#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include "qcustomplot.h"

class Spectrum : public QObject
{
public:
    explicit Spectrum(QCustomPlot* plot, QObject* parent = nullptr);
   ~Spectrum();

    void update(const QVector<double>& values);

private:
    Q_OBJECT
    QCustomPlot* plot;
    QCPBars* bars;
    QVector<double> x;
    int count = 0;
};

#endif // SPECTRUM_H
