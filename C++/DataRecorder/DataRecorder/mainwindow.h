#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "graph.h"
#include "sound.h"
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
    void update(int value);

    // void toggleDataProviderSignal();

public slots:
    void onSelected(const COMSettingsData& data);
    void onUpdate(int value);

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
    COMSettingsFile* file;
    Sound* clickSound;
    Sound* connectSound;
    bool isQuit = false;

    void setupGraph();
    void setupSounds();
    void setupCOMSettings();

    void print(const int value);
    void print(const QString& message);
};

#endif // MAINWINDOW_H
