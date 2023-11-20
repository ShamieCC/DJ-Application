/*
  ==============================================================================

    TrackList.cpp
    Created: 4 Mar 2023 4:11:40pm
    Author:  shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackList.h"
#include <filesystem>
//==============================================================================
TrackList::TrackList(juce::File _file) : file(_file), 
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    DBG("New track title: " << title);
}

bool TrackList::operator==(const juce::String& other) const 
{
    return title == other;
}