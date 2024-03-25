#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "graph.h"
#include "sound.h"
#include "comports.h"

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

private slots:
    void onSoundCompleted();
    void onConnect();
    void onClear();
    void onQuit();

private:
    void setupGraph();
    void setupSounds();
    void setupCOMPorts();
    void setupButtons();
    void print(const int value);
    void print(const QString& message);

    Q_OBJECT
    Ui::MainWindow* ui;
    Graph* graph;
    COMPorts* ports;
    Sound* clickSound;
    Sound* connectSound;
    Sound* disconnectSound;
    Sound* errorSound;
    bool isQuit = false;
};

#endif // MAINWINDOW_H
