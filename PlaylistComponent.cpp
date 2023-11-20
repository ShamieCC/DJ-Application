/*
  ==============================================================================

    PlaylistComponent.cpp
    Created:  5 Mar 2023 4:09:58pm
    Author:  shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, 
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _audioPlayer
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                       audioPlayer(_audioPlayer)
{
    // add and make the components visible
    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);
    addAndMakeVisible(library);
    addAndMakeVisible(loadToPlayer1);
    addAndMakeVisible(loadToPlayer2);

    // addingt listeners
    importButton.addListener(this);
    searchField.addListener(this);
    loadToPlayer1.addListener(this);
    loadToPlayer2.addListener(this);

    // searchField configuration
    searchField.setTextToShowWhenEmpty("Search Tracks (enter track name)", 
                                       juce::Colours::deeppink);
    searchField.onReturnKey = [this] { searchLibrary (searchField.getText()); };
    
    // load library from file
    library.getHeader().addColumn("Tracks", 1, 1);
    library.getHeader().addColumn("Duration", 2, 1);
    library.getHeader().addColumn("Delete", 3, 1);
    library.setModel(this);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
    saveLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    g.setColour (juce::Colours::grey);

    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);   

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains
    importButton.setBounds(0, 0, getWidth(), (getHeight()/ 8) + 10);
    library.setBounds(0, (1 * getHeight() / 16) + 15, getWidth() - 5, (13 * getHeight() / 16) - 65);
    searchField.setBounds(0, (14 * getHeight() / 16) - 50, getWidth(), (getHeight() / 16) + 15);
    loadToPlayer1.setBounds(0, (15 * getHeight() / 16) - 40, getWidth() / 2, (getHeight() / 16) + 40);
    loadToPlayer2.setBounds((getWidth() / 2) + 2, (15 * getHeight() / 16) - 40, getWidth() / 2, (getHeight() / 16) + 40);  

    //set columns
    library.getHeader().setColumnWidth(1, 12.8 * getWidth() / 20);
    library.getHeader().setColumnWidth(2, 5 * getWidth() / 20);
    library.getHeader().setColumnWidth(3, 2 * getWidth() / 20);
}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected
                                          )
{
    // highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::deeppink);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected
                                 )
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new juce::TextButton{ "DEL" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToLibrary();
        library.updateContent();
    }
    else if (button == &loadToPlayer1)
    {
        DBG("load to Player 1 clicked");
        loadInDeck(deckGUI1);
    }
    else if (button == &loadToPlayer2)
    {
        DBG("load to Player 2 clicked");
        loadInDeck(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].title + " removed from Library");
        deleteEntry(id);
        library.updateContent();
    }
}

void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)
{
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Loading: " << tracks[selectedRow].title << " to Player");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    
}

void PlaylistComponent::importToLibrary()
{
    DBG("importToLibrary called");


    auto fileChooserFlags =
        FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {

            for (const juce::File& file : chooser.getResults())
            {
                juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
                if (!checkTracks(fileNameWithoutExtension)) 
                {
                    TrackList newTrack{ file };
                    juce::URL audioURL{ file };
                    newTrack.length = getLength(audioURL);
                    tracks.push_back(newTrack);
                    DBG("file loaded: " << newTrack.title);
                }
              
            }

        });

}

bool PlaylistComponent::checkTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteEntry(int id)
{
    tracks.erase(tracks.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    audioPlayer->loadURL(audioURL);
    double seconds{ audioPlayer->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //convert seconds and minutes to string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };
    
    if (sec.length() < 2) 
    {
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    DBG("Search library for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = searchTracks(searchText);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

int PlaylistComponent::searchTracks(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(tracks.begin(), tracks.end(), 
        [&searchText](const TrackList& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracks.end())
    {
        i = std::distance(tracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create a csv file to save library
    std::ofstream myLibrary("my-library.csv");

    // save library to file
    for (TrackList& t : tracks)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myLibrary("my-library.csv");
    std::string filePath;
    std::string length;

    // Read data in every line 
    if (myLibrary.is_open())
    {
        while (getline(myLibrary, filePath, ',')) {
            juce::File file{ filePath };
            TrackList newTrack{ file };

            getline(myLibrary, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    myLibrary.close();
}
