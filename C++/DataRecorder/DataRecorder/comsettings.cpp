#include "comsettings.h"
#include "ui_comsettings.h"

COMSettings::COMSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::COMSettings)
{
    ui->setupUi(this);
    ui->ports->addItems({"COM 1", "COM 2"});
    ui->baudrates->addItems({"110", "256", "1024"});
}

COMSettings::~COMSettings()
{
    delete ui;
}

void COMSettings::onOK()
{

}

void COMSettings::onCancel()
{
    close();
}
