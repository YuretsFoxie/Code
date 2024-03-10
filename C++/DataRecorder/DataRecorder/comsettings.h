#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QDialog>
#include "sound.h"

namespace Ui {
class COMSettings;
}

class COMSettings : public QDialog
{
public:
    explicit COMSettings(QWidget *parent = nullptr);
    ~COMSettings();

private slots:
    void onOK();
    void onCancel();
    void onComboboxSelected();

private:
    Q_OBJECT
    Ui::COMSettings *ui;
    Sound* clickSound;
    Sound* comboboxSound;
};

#endif // COMSETTINGS_H
