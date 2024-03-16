#ifndef TESTDATAPROVIDER_H
#define TESTDATAPROVIDER_H

#include <QObject>
#include <QTimer>

class TestDataProvider: public QObject
{
public:
    TestDataProvider();
   ~TestDataProvider();

signals:
    void updateSignal(int value);

public slots:
    void toggleSlot();

private slots:
    void onTimer();

private:
    Q_OBJECT
    QTimer* timer;
    double n = 0;
};

#endif // TESTDATAPROVIDER_H
