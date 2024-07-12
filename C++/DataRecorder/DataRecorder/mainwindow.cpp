#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

// Public Functions

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupGraphs();
    setupSounds();
    setupCOMPorts();
    setupButtons();
    setupGenerator();
    setupSpectrumAnalyzer();
    setupFilter();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scope;
    delete spectrum;
    delete ports;
    delete connectSound;
    delete disconnectSound;
    delete clickSound;
    delete errorSound;
    delete generator;
    delete spectrumAnalyzer;
    delete filter;
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
    scope->push(value);
    // spectrumAnalyzer->push(value);
    // print(value);
}

void MainWindow::onFFTReady(const QVector<double> &result)
{
    spectrum->update(result);
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
    scope->clear();
    spectrum->clear();
}

void MainWindow::onQuit()
{
    clickSound->play();
    isQuit = true;
}

void MainWindow::setupGraphs()
{
    scope = new Scope(ui->scope);
    spectrum = new Spectrum(ui->spectrum);
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
    connect(spectrumAnalyzer, SpectrumAnalyzer::notifyFFTReady, this, onFFTReady);
}

void MainWindow::setupFilter()
{
    filter = new Filter();
    //connect(filter, Filter::notifyValue, this, onUpdateValue);
    //connect(ports, COMPorts::notifyValue, filter, Filter::onUpdateValue);
    //connect(generator, SignalGenerator::notifyValue, filter, Filter::onUpdateValue);
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
