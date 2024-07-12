#include "filter.h"

Filter::Filter(QObject *parent): QObject{parent}
{
    results.reserve(channelsNumber);
}

void Filter::onUpdateValue(const int value)
{
    results[currentChannel] += value;
    currentChannel++;

    if (currentChannel == channelsNumber)
    {
        currentChannel = 0;
        currentValue ++;
    }

    if (currentValue == valuesNumber - 1)
    {
        emit notifyValue(results[currentChannel] / valuesNumber);
        results[currentChannel] = 0;
    }

    if (currentValue == valuesNumber)
        currentValue = 0;
}



// c1v1, c2v1, c3v1, c4v1
// c1v2, c2v2, c3v2, c4v2
// c1v3, c2v3, c3v3, c4v3
// c1v4, c2v4, c3v4, c4v4



/*
class WorkerThread : public QThread {
    void run() {
        while(1) {
            // ... hard work
            // Now want to notify main thread:
            emit progressChanged("Some info");
        }
    }
    // Define signal:
signals:
    void progressChanged(QString info);
};



class MyWidget : public QWidget {
    // Your gui code

    // Define slot:
public slots:
    void onProgressChanged(QString info);
};



void MyWidget::onProgressChanged(QString info) {
    // Processing code
    textBox->setText("Latest info: " + info);
}



void MyWidget::startWorkInAThread() {
    // Create an instance of your woker
    WorkerThread *workerThread = new WorkerThread;
    // Connect our signal and slot
    connect(workerThread, SIGNAL(progressChanged(QString)),
            SLOT(onProgressChanged(QString)));
    // Setup callback for cleanup when it finishes
    connect(workerThread, SIGNAL(finished()),
            workerThread, SLOT(deleteLater()));
    // Run, Forest, run!
    workerThread->start(); // This invokes WorkerThread::run in a new thread
}
*/
