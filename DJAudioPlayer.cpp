/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created:  13 Feb 2023 4:10:38pm
    Author:  shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager
                            ) : formatManager(_formatManager)
{
    //reverb settings
    reverbParameters.roomSize = 0;
    reverbParameters.damping = 0;
    reverbParameters.wetLevel = 0;
    reverbParameters.dryLevel = 1.0;
    reverbSource.setParameters(reverbParameters);
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG("loadURL called");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) 
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::play()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("setPositionRelative position should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.25 || ratio > 4.0)
    {
        DBG("setSpeed ratio should be between 0.25 and 4");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setRoom(float size)
{
    reverbParameters.roomSize = size;
    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::setDamp(float dampingAmt)
{
    reverbParameters.damping = dampingAmt;
    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::setWet(float wet)
{
    reverbParameters.wetLevel = wet;
    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::setDry(float dry)
{
    reverbParameters.dryLevel = dry;
    reverbSource.setParameters(reverbParameters);
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}
