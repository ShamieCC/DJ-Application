/*
  ==============================================================================

    WaveformDisplay.cpp
    Created:  18 Feb 2023 9:34:31am
    Author:  shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id,
                                 AudioFormatManager& formatManager,
                                 AudioThumbnailCache& thumbCache
                                ) : audioThumb(1000, formatManager, thumbCache),
                                    fileLoaded(false),
                                    position(0),
                                    id(_id)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); 
    g.setColour (juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);  
    g.setColour(juce::Colours::deepskyblue);
    g.setFont(18.0f);
    g.drawText("Deck: " + std::to_string(id), getLocalBounds(),
            juce::Justification::centredLeft, true);

    if (fileLoaded)
    {
        g.setFont(20.0f);
        audioThumb.drawChannel(g, 
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f
                              );
        g.setColour(Colours::darkred);
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
        g.setColour(Colours::white);
        g.drawText(fileName, getLocalBounds(),
            juce::Justification::bottomLeft, true);
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);  
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        fileName = audioURL.getFileName();
        repaint();
    }
    else
    {
        DBG("file not loaded....");
    }
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
