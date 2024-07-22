/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 18 Aug 2023 5:51:30pm
    Author:  James

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track title", 1, 600);
    tableComponent.getHeader().addColumn("Deck 1", 3, 100); //load audio btn for deck 1
    tableComponent.getHeader().addColumn("Deck 2", 4, 100); //load audio btn for deck 2
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    loadTracksButton.setButtonText("Load Tracks from File");
    loadTracksButton.onClick = [this] { loadTracksButtonClicked(); };
    addAndMakeVisible(loadTracksButton);

    searchBox.setTextToShowWhenEmpty("Find a track", Colour::fromRGB(128, 128, 128));
    addAndMakeVisible(searchBox);
    searchButton.setButtonText("Search");
    searchButton.onClick = [this] { searchForTrack(searchBox.getText()); };
    addAndMakeVisible(searchButton);
}

PlaylistComponent::~PlaylistComponent()
{
    tableComponent.setModel(nullptr); //nullptr tablelistbox exception
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    //btn and search box dimensions
    const int buttonHeight = 27;
    const int buttonWidth = 200;
    const int searchBoxWidth = 90;
    const int searchButtonWidth = 50;

    //set bounds for the btns and search box
    loadTracksButton.setBounds((getWidth() - buttonWidth) / 2, 0, buttonWidth, buttonHeight);
    searchBox.setBounds(10, 0, searchBoxWidth, buttonHeight);
    searchButton.setBounds(15 + searchBoxWidth, 0, searchButtonWidth, buttonHeight);

    //adjust table to fit the rest of the available height
    tableComponent.setBounds(0, buttonHeight + 5, getWidth(), getHeight() - buttonHeight - 5);

    int tableWidth = tableComponent.getWidth();
    //column widths based on proportions
    tableComponent.getHeader().setColumnWidth(1, tableWidth * 0.6);   // 60% of the table width for "Track title"
    tableComponent.getHeader().setColumnWidth(3, tableWidth * 0.2);   // 20% of the table width for "Deck 1"
    tableComponent.getHeader().setColumnWidth(4, tableWidth * 0.2);   // 20% of the table width for "Deck 2"
}

int PlaylistComponent::getNumRows(){
    return tracks.size();
}
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    bool isSearchResult = std::find(searchResults.begin(), searchResults.end(), rowNumber) != searchResults.end();

    if (rowIsSelected)
    {
        g.fillAll(Colour::fromRGB(168, 38, 58));
    }
    else if (isSearchResult)
    {
        g.fillAll(Colour::fromRGB(38, 168, 148));
    }
    else
    {
        g.fillAll(Colour::fromRGB(101, 115, 126));
    }
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.drawText(tracks[rowNumber].title,2,0,width-4, height, Justification::centredLeft,true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    // handle buttons
    TextButton* btn = dynamic_cast<TextButton*>(existingComponentToUpdate);

    if (btn == nullptr)  // If we don't have a button yet, or the existing component isn't a button, we'll create one.
    {
        switch (columnId)
        {
        case 3:
            btn = new TextButton{ "Load 1" };
            break;
        case 4:
            btn = new TextButton{ "Load 2" };
            break;
        default:
            return existingComponentToUpdate;  // If it's not one of the columns we're interested in, return the existing component.
        }
    }
    // set ID and listener
    String id{ std::to_string(rowNumber) };
    btn->setComponentID(id);
    btn->addListener(this);

    return btn;
}

//load audio to deckGUI btn
void PlaylistComponent::buttonClicked(Button* button)
{
    int rowId = std::stoi(button->getComponentID().toStdString());

    //ensure rowId is a valid index before accessing the vector
    if (rowId >= 0 && rowId < tracks.size())
    {
        auto& track = tracks[rowId];
        URL filePathURL(track.filePath);
        String title = track.title;

        if (button->getButtonText() == "Load 1")
        {
            playlistListeners.call([&](PlaylistComponentListener& l)
                {
                    l.loadTrackToDeck1(filePathURL,title); //load the track to Deck 1
                });
        }
        else if (button->getButtonText() == "Load 2")
        {
            playlistListeners.call([&](PlaylistComponentListener& l)
                {
                    l.loadTrackToDeck2(filePathURL,title); //load the track to Deck 2
                });
        }
    }
}

void PlaylistComponent::loadTracksFromDirectory(const juce::File& directory)
{
    tracks.clear();

    //get all the audio files from the directory
    Array<File> audioFiles;
    directory.findChildFiles(audioFiles, File::findFiles, false, "*.wav;*.mp3;*.aiff");

    for (const auto& file : audioFiles)
    {
        TrackInfo track;
        track.title = file.getFileName().toStdString();
        track.filePath = file;
        tracks.push_back(track);
    }

    tableComponent.updateContent(); //update the table component content
}

void PlaylistComponent::loadTracksButtonClicked()
{
    FileChooser chooser("Select a directory with audio files");

    if (chooser.browseForDirectory()) //please include 'JUCE_MODAL_LOOPS_PERMITTED=1' under the preprocessor definitions in exporter settings.
    {
        File selectedDirectory = chooser.getResult();
        loadTracksFromDirectory(selectedDirectory);
    }
}

//to add or remove registered listeners
void PlaylistComponent::addPlaylistListener(PlaylistComponentListener* listener)
{
    playlistListeners.add(listener);
}

void PlaylistComponent::removePlaylistListener(PlaylistComponentListener* listener)
{
    playlistListeners.remove(listener);
}

//search function
void PlaylistComponent::searchForTrack(const String& query)
{
    searchResults.clear();
        //if search box is not empty
        if(!searchBox.isEmpty())
        {
            for (int i = 0; i < tracks.size(); ++i)
            {
                //if string contains substring push into searchResults vector
                if (tracks[i].title.containsIgnoreCase(query))
                {
                    searchResults.push_back(i);
                }
            }
        }
    //refresh the table to visually indicate the found results
    tableComponent.updateContent();
    tableComponent.repaint();
}