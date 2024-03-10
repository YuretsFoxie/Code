#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QAudioOutput>

class Sound: public QObject
{
public:
    Sound() {}
    Sound(const QString& name);
    void play();

signals:
    void completionSignal();

private slots:
    void completionSlot(QMediaPlayer::PlaybackState newState);

private:
    Q_OBJECT
    QMediaPlayer* player;
    QAudioOutput* output;
};

#endif // SOUND_H
