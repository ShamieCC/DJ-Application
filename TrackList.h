/*
  ==============================================================================

    TrackList.h
    Created: 4 Mar 2023 4:11:40pm
    Author:  shamie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
class TrackList
{
    public:
        /** construct reading a datafile*/
        TrackList(File _file);

        File file;
        URL URL;
        String title;
        String length;
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;
};