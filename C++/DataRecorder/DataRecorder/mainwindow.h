#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "sound.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSoundCompleted();
    void onConnect();
    void onSettings();
    void onTest();
    void onQuit();

private:
    Q_OBJECT
    Ui::MainWindow* ui;
    QCPGraph* graph;
    Sound* clickSound;
    bool isQuit = false;

    void configurePlot();
};

#endif // MAINWINDOW_H
