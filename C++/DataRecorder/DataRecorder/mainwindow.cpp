#include "mainwindow.h"
#include "ui_mainwindow.h"

// Public Functions

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupGraph();
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

void MainWindow::onSelected(const COMSettingsData& data)
{
    file->save(data);
}

void MainWindow::onUpdate(int value)
{
    emit update(value);
}

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
    clickSound->play();
}

void MainWindow::onQuit()
{
    clickSound->play();
    isQuit = true;
}

void MainWindow::setupGraph()
{
    graph = new Graph(this);
    graph->setPlot(ui->plot);
    connect(this, update, graph, Graph::onUpdate);
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

void MainWindow::print(const int value)
{
    print(tr("%1").arg(value));
}

void MainWindow::print(const QString& message)
{
    QString time = QTime::currentTime().toString();
    ui->textBrowser->append("      " + time + "    " + message);
}
