#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comsettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configurePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configurePlot()
{
    QCustomPlot* plot = ui->plot;

    int size = 100;
    QVector<double> xValues(size), yValues(size);

    int count = 0;
    for (double i = 0; i < 10.0; i+= 0.1, count++)
    {
        xValues[count] = count;
        yValues[count] = sin(i) * 4;
    }

    graph = plot->addGraph();
    graph->setData(xValues, yValues);

    //=====

    graph->setPen(QPen(Qt::green, 1));

    plot->setBackground(Qt::transparent);
    plot->xAxis->setBasePen(QPen(Qt::green, 1));
    plot->yAxis->setBasePen(QPen(Qt::green, 1));
    plot->xAxis->setTickPen(QPen(Qt::green, 1));
    plot->yAxis->setTickPen(QPen(Qt::green, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->xAxis->setTickLabelColor(Qt::green);
    plot->yAxis->setTickLabelColor(Qt::green);

    plot->xAxis->grid()->setPen(QPen(Qt::green, 1, Qt::DotLine));
    plot->yAxis->grid()->setPen(QPen(Qt::green, 1, Qt::DotLine));

    plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

    plot->rescaleAxes();
    plot->xAxis->setRange(0, 100);
    plot->yAxis->setRange(-4, 4);
}

void MainWindow::onQuit()
{
    QApplication::quit();
}

void MainWindow::onSettings()
{
    COMSettings* settings = new COMSettings();
    settings->setModal(true);
    settings->setWindowFlags(Qt::FramelessWindowHint);
    settings->show();
}
