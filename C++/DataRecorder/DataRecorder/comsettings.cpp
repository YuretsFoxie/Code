#include "comsettings.h"
#include "ui_comsettings.h"

COMSettings::COMSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::COMSettings)
{
    ui->setupUi(this);
    ui->ports->addItems({"COM 1", "COM 2"});
    ui->baudrates->addItems({"110", "256", "1024"});
    clickSound = new Sound("buttonclick");
    comboboxSound = new Sound("buttonrollover");

    connect(ui->okButton, QPushButton::clicked, this, onOK);
    connect(ui->cancelButton, QPushButton::clicked, this, onCancel);
    connect(ui->ports, QComboBox::activated, this, onComboboxSelected);
    connect(ui->baudrates, QComboBox::activated, this, onComboboxSelected);
}

COMSettings::~COMSettings()
{
    delete clickSound;
    delete comboboxSound;
    delete ui;
}

void COMSettings::onOK()
{
    clickSound->play();
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
