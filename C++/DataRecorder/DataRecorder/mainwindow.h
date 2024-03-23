#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "graph.h"
#include "sound.h"
#include "comports.h"
#include "comsettings.h"
#include "comsettingsfile.h"

namespace Ui
{
    class MainWindow;
}



class MainWindow: public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void select(const COMSettingsData& data);
    void updateGraph(const int value);
    void toggleCOMPort(const COMSettingsData& data);
    void transmit(const int value);

public slots:
    void onSelected(const COMSettingsData& data);
    void onUpdateConnected();
    void onUpdateDisconnected();
    void onUpdateMessage(const QString& message);
    void onUpdateValue(const int value);

private slots:
    void onSoundCompleted();
    void onConnect();
    void onSettings();
    void onTest();
    void onQuit();

private:
    Q_OBJECT
    Ui::MainWindow* ui;
    Graph* graph;
    COMSettings* settings;
    COMPorts* ports;
    COMSettingsFile* file;
    Sound* clickSound;
    Sound* connectSound;
    Sound* disconnectSound;
    bool isQuit = false;

    void setupGraph();
    void setupSounds();
    void setupCOMPorts();
    void setupCOMSettings();

    void print(const int value);
    void print(const QString& message);
};

#endif // MAINWINDOW_H
