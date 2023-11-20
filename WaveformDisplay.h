/*
  ==============================================================================

    WaveformDisplay.h
    Created: 18 Feb 2023 9:34:31am
    Author:  shamie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                         public ChangeListener
{
public:
    WaveformDisplay(int _id,
                    AudioFormatManager& formatManager,
                    AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /*loads the audio file */
    void loadURL(URL audioURL);
    /**set the relative position of the playhead*/
    void setPositionRelative(double pos);

private:
    int id;
    bool fileLoaded;
    double position;
    String fileName;
    AudioThumbnail audioThumb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
