#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QDialog>

namespace Ui {
class COMSettings;
}

class COMSettings : public QDialog
{
    Q_OBJECT

public:
    explicit COMSettings(QWidget *parent = nullptr);
    ~COMSettings();

private slots:
    void onOK();
    void onCancel();

private:
    Ui::COMSettings *ui;
};

#endif // COMSETTINGS_H
