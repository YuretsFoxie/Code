#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scope.h"
#include "spectrum.h"
#include "sound.h"
#include "comports.h"
#include "spectrumanalyzer.h"
#include "signalgenerator.h"

namespace Ui
{
    class MainWindow;
}



class MainWindow: public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onUpdateConnected();
    void onUpdateDisconnected();
    void onUpdateMessage(const QString& message);
    void onUpdateError(const QString& message);
    void onUpdateValue(const int value);
    void onFFTReady(const QVector<double>& result);

private slots:
    void onSoundCompleted();
    void onConnect();
    void onTest();
    void onClear();
    void onQuit();

private:
    void setupGraphs();
    void setupSounds();
    void setupCOMPorts();
    void setupButtons();
    void setupGenerator();
    void setupSpectrumAnalyzer();
    void print(const int value);
    void print(const QString& message);

    Q_OBJECT
    Ui::MainWindow* ui;
    Scope* scope;
    Spectrum* spectrum;
    COMPorts* ports;
    SignalGenerator* generator;
    SpectrumAnalyzer* spectrumAnalyzer;
    Sound* clickSound;
    Sound* connectSound;
    Sound* disconnectSound;
    Sound* errorSound;
    bool isQuit = false;
};

#endif // MAINWINDOW_H
