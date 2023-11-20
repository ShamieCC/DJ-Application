/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Feb 2023 4:10:38pm
    Author:  shamie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
class DJAudioPlayer : public AudioSource
{
    public:
        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        /**Loads audio file*/
        void loadURL(URL audioURL);
        /**Plays audio */
        void play();
        /**Stops playing audio */
        void stop();
        /**Sets relative position of audio */
        void setPositionRelative(double pos);
        /**Sets the volume */
        void setGain(double gain);
        /**Sets the speed*/
        void setSpeed(double ratio);
        /**Gets relative position */
        double getPositionRelative();
        /**Gets the length in seconds*/
        double getLengthInSeconds();
        /**Sets the room size*/
        void setRoom(float size);
        /**sets the damp level*/
        void setDamp(float damp);
        /**sets the wet level*/
        void setWet(float wet);
        /**sets the dry level*/
        void setDry(float dry);

    private:
        void setPosition(double posInSecs);
        AudioFormatManager& formatManager;
        std::unique_ptr<AudioFormatReaderSource> readerSource;
        AudioTransportSource transportSource;
        ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
        ReverbAudioSource reverbSource{ &resampleSource, false };
        Reverb::Parameters reverbParameters;
};
