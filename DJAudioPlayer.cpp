/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            :formatManager(_formatManager)
{}

DJAudioPlayer::~DJAudioPlayer()
{}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
        sendChangeMessage();
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain >= 0.1 && gain <= 1.0)
    {
        transportSource.setGain(gain);
    }
    else
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0.1 and 1" << std::endl;
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio >= 0 && ratio <= 100.0)
    {
        resampleSource.setResamplingRatio(ratio);
    }
    else
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
    playing = true;
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
    playing = false;
}

bool DJAudioPlayer::isPlaying() const
{
    return playing;
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setPosition(float posInSecs)
{
    transportSource.setPosition(posInSecs);
}

//set audio position by disc rotation, angleDifference is broadcasted from the TurntableComponent
void DJAudioPlayer::setPosByDiscRotation(float angleDifference)
{
    double currentAudioPos = transportSource.getCurrentPosition();
    double newAudioPos = currentAudioPos + angleDifference;
    
    if (newAudioPos < 0)
    {
        newAudioPos = 0;
    }
    else if (newAudioPos > transportSource.getLengthInSeconds())
    {
        newAudioPos = transportSource.getLengthInSeconds();
    }
    setPosition(newAudioPos);
}
