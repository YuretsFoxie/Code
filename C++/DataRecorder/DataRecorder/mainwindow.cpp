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
    setupCOMPorts();
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

void MainWindow::onUpdateConnected()
{
    connectSound->play();
    ui->connectLabel->setText("Online");
}

void MainWindow::onUpdateDisconnected()
{
    disconnectSound->play();
    ui->connectLabel->setText("");
}

void MainWindow::onUpdateMessage(const QString& message)
{
    print(message);
}

void MainWindow::onUpdateError(const QString &message)
{
    onUpdateMessage("error: " + message);
    errorSound->play();
}

void MainWindow::onUpdateValue(const int value)
{
    emit updateGraph(value);
}

// Private Functions

void MainWindow::onSoundCompleted()
{
    if (isQuit)
        QApplication::quit();
}

void MainWindow::onConnect()
{
    emit toggleCOMPort(file->load());
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
    connect(this, updateGraph, graph, Graph::onUpdate);
}

void MainWindow::setupSounds()
{
    connectSound = new Sound("suitchargeok1");
    disconnectSound = new Sound("suitchargeno1");
    clickSound = new Sound("buttonclick");
    errorSound = new Sound("warn1");
    connect(clickSound, Sound::completionSignal, this, onSoundCompleted);
}

void MainWindow::setupCOMPorts()
{
    ports = new COMPorts();
    connect(this, toggleCOMPort, ports, COMPorts::onToggle);
    connect(this, transmit, ports, COMPorts::onTransmit);
    connect(ports, COMPorts::notifyConnected, this, onUpdateConnected);
    connect(ports, COMPorts::notifyDisconnected, this, onUpdateDisconnected);
    connect(ports, COMPorts::notifyMessage, this, onUpdateMessage);
    connect(ports, COMPorts::notifyError, this, onUpdateError);
    connect(ports, COMPorts::notifyValue, this, onUpdateValue);
}

void MainWindow::setupCOMSettings()
{
    file = new COMSettingsFile();
    settings = new COMSettings(this);
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
