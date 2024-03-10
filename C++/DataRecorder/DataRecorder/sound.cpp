#include "sound.h"

Sound::Sound(const QString& name)
{
    player = new QMediaPlayer();
    output = new QAudioOutput();
    QString url = "qrc:/Resources/sounds/" + name + ".wav";

    player->setAudioOutput(output);
    player->setSource(QUrl(url));
    output->setVolume(100);

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
