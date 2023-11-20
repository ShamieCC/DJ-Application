/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Feb 2023 4:08:07pm
    Author:  shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player, 
                 juce::AudioFormatManager& formatManager,
                 juce::AudioThumbnailCache& thumbCache
                ) : player(_player),
                    id(_id),
                    waveformDisplay(id, formatManager, thumbCache)
{
    // add and make the components visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
  
    addAndMakeVisible(roomSlider);
    addAndMakeVisible(dampSlider);
    addAndMakeVisible(wetSlider);
    addAndMakeVisible(drySlider);

    addAndMakeVisible(waveformDisplay);

    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    reverbSlider.addListener(this);
  
    roomSlider.addListener(this);
    dampSlider.addListener(this);
    wetSlider.addListener(this);
    drySlider.addListener(this);

    //configure volume slider and label
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, 
                              false, 
                              70, 
                              volSlider.getTextBoxHeight()
                             );
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); 
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                              false,
                              50,
                              speedSlider.getTextBoxHeight()
                             );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                              false,
                              50,
                              posSlider.getTextBoxHeight()
                             );
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);

    //configure reverb slider
    reverbSlider.setRange(0.0, 1.0);
    reverbSlider.setNumDecimalPlacesToDisplay(2);

    roomSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    roomSlider.setRange(0.0, 1.0);
    roomSlider.setNumDecimalPlacesToDisplay(2);
    roomSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, roomSlider.getTextBoxHeight());
    roomSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);


    dampSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    dampSlider.setRange(0.0, 1.0);
    dampSlider.setNumDecimalPlacesToDisplay(2);
    dampSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, dampSlider.getTextBoxHeight());
    dampSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);

    wetSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    wetSlider.setRange(0.0, 1.0);
    wetSlider.setNumDecimalPlacesToDisplay(2);
    wetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, wetSlider.getTextBoxHeight());
    wetSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);

    drySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    drySlider.setValue(0.5);
    drySlider.setRange(0.0, 1.0);
    drySlider.setNumDecimalPlacesToDisplay(2);
    drySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, posSlider.getTextBoxHeight());
    drySlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
   
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    g.setColour (juce::Colours::grey);
    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);   
}

void DeckGUI::resized()
{
    /*This method is where you should set the bounds of any child
    components that your component contains..*/
    playButton.setBounds(5, (4 * getHeight() / 8) + 130, 70, getHeight() / 8);
    stopButton.setBounds(80 , (4 * getHeight() / 8) + 130, 70, getHeight() / 8);
    loadButton.setBounds(155, (4 * getHeight() / 8) + 130, 70, getHeight() / 8);
    volSlider.setBounds(290, (4 * getHeight() / 8) + 13, 50, 160);
    speedSlider.setBounds(355, (4 * getHeight() / 8) +13, 50, 160);
    posSlider.setBounds(415, (4 * getHeight() / 8) +13, 50, 160);
    waveformDisplay.setBounds(0, 0,  getWidth(), 70);
 
    roomSlider.setBounds(0, 72, 175, 110);
    dampSlider.setBounds(140, 72, 175, 110);
    wetSlider.setBounds(0, 190, 175, 110);
    drySlider.setBounds(140, 190,175,  110);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked ");
        player->play();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton)
    {

        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                player->loadURL(URL{ chooser.getResult() });
        
        waveformDisplay.loadURL(URL{ chooser.getResult() });
            });

    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        DBG("Volume slider moved " << slider->getValue());
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        DBG("Speed slider moved " << slider->getValue());
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        DBG("Position slider moved " << slider->getValue());
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &roomSlider)
    {
        player->setRoom(slider->getValue());
        
    }
    if (slider == &dampSlider)
    {
        player->setDry(slider->getValue());
    }
    if (slider == &wetSlider)
    {
        player->setWet(slider->getValue());
    }
    if (slider == &drySlider)
    {
        player->setDry(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("InterestedInFileDrag called. " 
        + std::to_string(files.size()) + " file(s) being dragged.");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    DBG("filesDropped at " + std::to_string(x) 
        + "x and " + std::to_string(y) + "y" );
    if (files.size() == 1)
    {
        loadFile(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{   
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}
