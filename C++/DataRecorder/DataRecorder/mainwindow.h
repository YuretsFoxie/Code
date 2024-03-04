#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void configurePlot();

private slots:
    void onQuit();
    void onSettings();

private:
    Ui::MainWindow* ui;
    QCPGraph* graph;
};

#endif // MAINWINDOW_H
