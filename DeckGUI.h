/*
  ==============================================================================

    DeckGUI.h
    Created: 14 Feb 2023 4:08:07pm
    Author:  shamie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;
//==============================================================================
/*
*/
class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
               
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player, 
            AudioFormatManager& formatManager, 
            AudioThumbnailCache& thumbCache);
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    /**Button::Listener*/
    void buttonClicked(Button* button) override;
    /**Slider::Listener */
    void sliderValueChanged(Slider* slider) override;
    /*checks if file is being dragged over deck*/
    bool isInterestedInFileDrag(const StringArray& files) override;
    /**checks if file is dropped onto deck*/
    void filesDropped(const StringArray &files, int x, int y) override;
    /**elisteners for waveform chages*/
    void timerCallback() override;

private:

    int id;
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    Slider volSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow };
    Label volLabel;
    Slider speedSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow };
    Label speedLabel;
    Slider posSlider{Slider::SliderStyle::LinearVertical, Slider::TextBoxBelow };
    Label posLabel;
    Slider reverbSlider;
    Slider roomSlider;
    Slider dampSlider;
    Slider wetSlider;
    Slider drySlider;
    void loadFile(URL audioURL);

    FileChooser fChooser{ "Select a file..." };

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    SharedResourcePointer< TooltipWindow > sharedTooltip;

    friend class PlaylistComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
