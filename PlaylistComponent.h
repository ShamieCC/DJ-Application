/*
  ==============================================================================

    PlaylistComponent.h
    Created: 5 Mar 2023 4:09:58pm
    Author:  shamie

  ==============================================================================
*/

#pragma once
#include <vector>
#include <algorithm>
#include <fstream>
#include "TrackList.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
using namespace juce; 

//==============================================================================
/*
*/
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, 
                      DeckGUI* _deckGUI2, 
                      DJAudioPlayer* _audioPlayer
                     );
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected
                           ) override;
    void paintCell(Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                  ) override;
    
    Component* refreshComponentForCell(int rowNumber, 
                                       int columnId, 
                                       bool isRowSelected, 
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(Button* button) override;

private:
    std::vector<TrackList> tracks;
    TextButton importButton{ "IMPORT TRACKS TO PLAYLIST" };
    TextEditor searchField;
    TableListBox library;
    TextButton loadToPlayer1{ "LOAD TO DECK 1" };
    TextButton loadToPlayer2{ "LOAD TO DECK 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* audioPlayer;
    
    FileChooser fChooser{ "Select a file..." };

    String getLength(URL audioURL);
    String secondsToMinutes(double seconds);

    /*function prototypes*/
    void loadLibrary();
    void searchLibrary(String searchText);
    void importToLibrary();
    void saveLibrary(); 
    void deleteEntry(int id);
    void loadInDeck(DeckGUI* deckGUI);

    bool checkTracks(String fileNameWithoutExtension);
    int searchTracks(String searchText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
