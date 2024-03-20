#ifndef COMSETTINGS_H
#define COMSETTINGS_H

#include <QDialog>
#include <QComboBox>
#include "sound.h"
#include "comsettingsdata.h"

namespace Ui {
    class COMSettings;
}



class COMSettings: public QDialog
{
public:
    explicit COMSettings(QWidget *parent = nullptr);
    ~COMSettings();

signals:
    void select(const COMSettingsData& data);

public slots:
    void onSelected(const COMSettingsData& data);

private slots:
    void onOK();
    void onCancel();
    void onComboboxSelected();

private:
    Q_OBJECT
    Ui::COMSettings *ui;
    Sound* clickSound;
    Sound* comboboxSound;

    void setupComboboxes();
    void setupButtons();
    void setupSounds();
};

#endif // COMSETTINGS_H
