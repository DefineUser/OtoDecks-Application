/*
  ==============================================================================

    PlaylistComponent.h
    Created: 18 Aug 2023 5:51:30pm
    Author:  James

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/

//custom listener class for track loading
class PlaylistComponentListener
{
public:
    virtual ~PlaylistComponentListener() = default;

    //pure virtual functions
    virtual void loadTrackToDeck1(URL& trackURL, String title) = 0;
    virtual void loadTrackToDeck2(URL& trackURL, String title) = 0;
};

class PlaylistComponent  : public juce::Component, 
                           public TableListBoxModel, 
                           public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button * button) override;

    //load track functions
    void loadTracksFromDirectory(const juce::File& directory);
    void loadTracksButtonClicked();

    //search function
    void searchForTrack(const String& query);

    void addPlaylistListener(PlaylistComponentListener* listener);
    void removePlaylistListener(PlaylistComponentListener* listener);

private:
    TableListBox tableComponent;

    struct TrackInfo
    {
        String title;
        File filePath;
    };
    std::vector<TrackInfo> tracks;
    
    TextButton loadTracksButton;

    TextButton searchButton;
    TextEditor searchBox;
    std::vector<int> searchResults;

    ListenerList<PlaylistComponentListener> playlistListeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
