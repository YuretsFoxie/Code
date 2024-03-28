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
    setupButtons();
    setupGenerator();
    setupSpectrumAnalyzer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph;
    delete ports;
    delete connectSound;
    delete disconnectSound;
    delete clickSound;
    delete errorSound;
    delete generator;
    delete spectrumAnalyzer;
}

void MainWindow::onUpdateConnected()
{
    connectSound->play();
    ui->connectButton->setText("Disconnect");
    ui->connectLabel->setText("Online");
}

void MainWindow::onUpdateDisconnected()
{
    disconnectSound->play();
    ui->connectButton->setText("Connect");
    ui->connectLabel->setText("");
}

void MainWindow::onUpdateMessage(const QString& message)
{
    print(message);
}

void MainWindow::onUpdateError(const QString &message)
{
    print("error: " + message);
    errorSound->play();
}

void MainWindow::onUpdateValue(const int value)
{
    graph->update(value);
}

// Private Functions

void MainWindow::onSoundCompleted()
{
    if (isQuit)
        QApplication::quit();
}

void MainWindow::onConnect()
{
    ports->toggle();
}

void MainWindow::onTest()
{
    connectSound->play();
    generator->toggle();
}

void MainWindow::onClear()
{
    clickSound->play();
    ui->textBrowser->clear();
    graph->clear();
}

void MainWindow::onQuit()
{
    clickSound->play();
    isQuit = true;
}

void MainWindow::setupGraph()
{
    graph = new Graph(ui->plot);
}

void MainWindow::setupSounds()
{
    connectSound = new Sound("battery_pickup");
    disconnectSound = new Sound("battery_pickup");
    clickSound = new Sound("buttonclick");
    errorSound = new Sound("warn1");
    connect(clickSound, Sound::completionSignal, this, onSoundCompleted);
}

void MainWindow::setupCOMPorts()
{
    ports = new COMPorts();
    connect(ports, COMPorts::notifyConnected, this, onUpdateConnected);
    connect(ports, COMPorts::notifyDisconnected, this, onUpdateDisconnected);
    connect(ports, COMPorts::notifyMessage, this, onUpdateMessage);
    connect(ports, COMPorts::notifyError, this, onUpdateError);
    connect(ports, COMPorts::notifyValue, this, onUpdateValue);
}

void MainWindow::setupButtons()
{
    ui->connectButton->setEnabled(ports->isPortAvailable());
    connect(ui->connectButton, QPushButton::clicked, this, onConnect);
    connect(ui->testButton, QPushButton::clicked, this, onTest);
    connect(ui->clearButton, QPushButton::clicked, this, onClear);
    connect(ui->quitButton, QPushButton::clicked, this, onQuit);
}

void MainWindow::setupGenerator()
{
    generator = new SignalGenerator();
    connect(generator, SignalGenerator::notifyValue, this, onUpdateValue);
}

void MainWindow::setupSpectrumAnalyzer()
{
    spectrumAnalyzer = new SpectrumAnalyzer();
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
