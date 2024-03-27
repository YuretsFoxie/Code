#include "spectrumanalyzer.h"

// Public Functions

SpectrumAnalyzer::SpectrumAnalyzer(QObject *parent): QObject{parent} {}

void SpectrumAnalyzer::push(const int value)
{
    buffer.append(cd(value, 0));

    if (buffer.count() < n)
        return;

    if (buffer.count() > n)
        buffer.pop_front();

    QList<cd> fft = calculateFFT(buffer);
    QVector<double> magnitudes(n / 2);

    for(int i = 0; i < n / 2; i++)
        magnitudes[i] = abs(fft[i]);

    emit notifyFFTReady(magnitudes);    // The first magnitude is a DC offset
}

// Private Functions

SpectrumAnalyzer::SplittedItems SpectrumAnalyzer::split(const QList<cd>& v)
{
    QList<cd> evens, odds;
    int count = v.size();

    for(int i = 0; i < count; i++)
        (i % 2 ? odds : evens).push_back(v[i]);

    return {evens, odds};
}

QList<cd> SpectrumAnalyzer::calculateFFT(const QList<cd>& v)
{
    int n = v.size();

    if (n == 1)
        return v;

    SplittedItems items = split(v);

    QList<cd> evens = calculateFFT(items.evens);
    QList<cd> odds = calculateFFT(items.odds);
    QList<cd> result(n);

    cd w = exp(cd(-2 * pi / n) * i);

    for(int j = 0; j < n / 2; j++)
    {
        cd term = pow(w, j) * odds[j];
        result[j] = evens[j] + term;
        result[j + n / 2] = evens[j] - term;
    }

    return result;
}
