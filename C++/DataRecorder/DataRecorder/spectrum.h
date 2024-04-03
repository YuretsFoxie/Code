#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QObject>
#include "qcustomplot.h"
#include "settings.h"

class Spectrum: public QObject
{
public:
    explicit Spectrum(QCustomPlot* plot, QObject* parent = nullptr);
   ~Spectrum();

    void update(const QVector<double>& values);
    void clear();

private:
    Q_OBJECT
    QCustomPlot* plot;
    QCPBars* bars;
    QVector<double> x;
    QVector<double> clearY;
    int width = Settings::shared().getData().spectrumN / 2;
    int count = 0;
};

#endif // SPECTRUM_H
