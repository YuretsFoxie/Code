#include "mainwindow.h"
#include "ui_mainwindow.h"

// Public Functions

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupPlot();
    setupSounds();
    setupCOMSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete clickSound;
    delete file;
    delete settings;
}

void MainWindow::print(const int value)
{
    print(tr("%1").arg(value));
}

void MainWindow::print(const QString& message)
{
    QString time = QTime::currentTime().toString();
    ui->textBrowser->append("      " + time + "    " + message);
}

void MainWindow::onSelected(const COMSettingsData& data)
{
    file->save(data);
}

//=====
// TODO: use this function at the Graph class

/*
void MainWindow::updateSlot(int value)
{
    int count = graph->dataCount();
    graph->addData(count, value);
    ui->plot->xAxis->setRange(count + 1, 128, Qt::AlignRight);
    ui->plot->replot();
}
*/

//=====

// Private Functions

void MainWindow::onSoundCompleted()
{
    if (isQuit)
        QApplication::quit();
}

void MainWindow::onConnect()
{
    connectSound->play();
}

void MainWindow::onSettings()
{
    clickSound->play();
    settings->setModal(true);
    settings->setWindowFlags(Qt::FramelessWindowHint);
    settings->show();
    emit select(file->load());
}

void MainWindow::onTest()
{
    print("test");
    clickSound->play();
}

void MainWindow::onQuit()
{
    clickSound->play();
    isQuit = true;
}

void MainWindow::setupPlot()
{
    QCustomPlot* plot = ui->plot;
    graph = plot->addGraph();
    graph->setPen(QPen(Qt::green, 1));

    plot->setBackground(Qt::transparent);
    plot->xAxis->setBasePen(QPen(Qt::green, 1));
    plot->yAxis->setBasePen(QPen(Qt::green, 1));
    plot->xAxis->setTickPen(QPen(Qt::green, 1));
    plot->yAxis->setTickPen(QPen(Qt::green, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::green, 1));
    plot->xAxis->setTickLabelColor(Qt::transparent);
    plot->yAxis->setTickLabelColor(Qt::transparent);

    plot->xAxis->grid()->setPen(QPen(Qt::transparent));
    plot->yAxis->grid()->setPen(QPen(Qt::transparent));
    plot->xAxis->grid()->setZeroLinePen(QPen(Qt::transparent));
    plot->yAxis->grid()->setZeroLinePen(QPen(Qt::transparent));

    plot->rescaleAxes();
    plot->xAxis->setRange(-128, 0);
    plot->yAxis->setRange(-520, 520); // -512 - 511
}

void MainWindow::setupSounds()
{
    connectSound = new Sound("suitchargeok1");
    clickSound = new Sound("buttonclick");
    connect(clickSound, Sound::completionSignal, this, onSoundCompleted);
}

void MainWindow::setupCOMSettings()
{
    settings = new COMSettings(this);
    file = new COMSettingsFile();

    connect(settings, COMSettings::select, this, onSelected);
    connect(this, select, settings, COMSettings::onSelected);
}
