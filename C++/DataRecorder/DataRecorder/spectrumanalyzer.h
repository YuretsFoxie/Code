#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QObject>
#include <complex>
#include "settings.h"

using namespace std;
typedef complex<double> cd;



class SpectrumAnalyzer: public QObject
{
public:
    explicit SpectrumAnalyzer(QObject *parent = nullptr);
    void push(const int value);

signals:
    void notifyFFTReady(const QVector<double>& result);

private:
    struct SplittedItems
    {
        QList<cd> evens;
        QList<cd> odds;
    };

    SplittedItems split(const QList<cd>& v);
    QList<cd> calculateFFT(const QList<cd>& v);

    const int n = Settings::shared().getData().spectrumN; // n must be a power of two
    const double pi = acos(-1);
    const cd i = cd(0.0, 1.0);

    Q_OBJECT
    QList<cd> buffer;
};

#endif // SPECTRUMANALYZER_H
