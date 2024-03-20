#include <QSerialPortInfo>
#include <QList>
#include "comsettings.h"
#include "ui_comsettings.h"

// Public Functions

COMSettings::COMSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::COMSettings)
{
    ui->setupUi(this);

    setupComboboxes();
    setupButtons();
    setupSounds();
}

COMSettings::~COMSettings()
{
    delete clickSound;
    delete comboboxSound;
    delete ui;
}

void COMSettings::onSelected(const COMSettingsData& data)
{
    ui->ports->setCurrentIndex(data.portIndex);
    ui->baudrates->setCurrentIndex(data.baudrateIndex);
}

// Private Functions

void COMSettings::onOK()
{
    clickSound->play();

    int portIndex = ui->ports->currentIndex();
    int baudrateIndex = ui->baudrates->currentIndex();

    emit select({portIndex, baudrateIndex});
    close();
}

void COMSettings::onCancel()
{
    clickSound->play();
    close();
}

void COMSettings::onComboboxSelected()
{
    comboboxSound->play();
}

void COMSettings::setupComboboxes()
{
    QList<QString> portNames;
    QList<QString> baudrates;

    foreach (auto port, QSerialPortInfo::availablePorts())
        portNames.append(port.portName());

    foreach (auto item, QSerialPortInfo::standardBaudRates())
        baudrates.append(tr("%1").arg(item));

    ui->ports->addItems(portNames);
    ui->baudrates->addItems(baudrates);

    connect(ui->ports, QComboBox::activated, this, onComboboxSelected);
    connect(ui->baudrates, QComboBox::activated, this, onComboboxSelected);
}

void COMSettings::setupButtons()
{
    connect(ui->okButton, QPushButton::clicked, this, onOK);
    connect(ui->cancelButton, QPushButton::clicked, this, onCancel);
}

void COMSettings::setupSounds()
{
    clickSound = new Sound("buttonclick");
    comboboxSound = new Sound("buttonrollover");
}
