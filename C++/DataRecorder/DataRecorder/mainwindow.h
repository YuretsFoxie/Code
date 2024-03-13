#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "qcustomplot.h"
#include "comsettings.h"
#include "sound.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print(const QString& message);
    void onSettingsSelected(COMSettingsData data);

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
