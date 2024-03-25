#include "sound.h"

Sound::Sound(const QString& name, const int volume)
{
    player = new QMediaPlayer();
    output = new QAudioOutput();
    QString url = path + name + type;

    player->setAudioOutput(output);
    player->setSource(QUrl(url));
    output->setVolume(volume);

    connect(player, &QMediaPlayer::playbackStateChanged, this, completionSlot);
}

void Sound::play()
{
    player->play();
}

void Sound::completionSlot(QMediaPlayer::PlaybackState newState)
{
    if (newState == QMediaPlayer::PlaybackState::StoppedState)
        emit completionSignal();
}
