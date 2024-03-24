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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete clickSound;
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
    print("error: " + message);
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
    emit toggleCOMPort();
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
    graph = new Graph(this);
    graph->setPlot(ui->plot);
    connect(this, updateGraph, graph, Graph::onUpdate);
}

void MainWindow::setupSounds()
{
    connectSound = new Sound("battery_pickup");
    disconnectSound = new Sound("suitchargeno1");
    clickSound = new Sound("buttonclick");
    errorSound = new Sound("warn1");
    connect(clickSound, Sound::completionSignal, this, onSoundCompleted);
}

void MainWindow::setupCOMPorts()
{
    ports = new COMPorts();
    connect(this, toggleCOMPort, ports, COMPorts::onToggle);
    connect(ports, COMPorts::notifyConnected, this, onUpdateConnected);
    connect(ports, COMPorts::notifyDisconnected, this, onUpdateDisconnected);
    connect(ports, COMPorts::notifyMessage, this, onUpdateMessage);
    connect(ports, COMPorts::notifyError, this, onUpdateError);
    connect(ports, COMPorts::notifyValue, this, onUpdateValue);
}

void MainWindow::setupButtons()
{
    COMSettingsData data;
    QVector<QString> names;

    for (auto port: QSerialPortInfo::availablePorts())
        names.append(port.portName());

    bool isEnabled = names.contains(data.port);
    ui->connectButton->setEnabled(isEnabled);

    connect(ui->connectButton, QPushButton::clicked, this, onConnect);
    connect(ui->clearButton, QPushButton::clicked, this, onClear);
    connect(ui->quitButton, QPushButton::clicked, this, onQuit);
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
