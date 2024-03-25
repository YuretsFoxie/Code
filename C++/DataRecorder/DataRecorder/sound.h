#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

class Sound: public QObject
{
public:
    Sound(const QString& name, const int volume = 100);
    void play();

signals:
    void completionSignal();

private slots:
    void completionSlot(QMediaPlayer::PlaybackState newState);

private:
    const QString path = "qrc:/Resources/sounds/";
    const QString type = ".wav";

    Q_OBJECT
    QMediaPlayer* player;
    QAudioOutput* output;
};

#endif // SOUND_H
