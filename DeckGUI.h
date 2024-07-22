/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TurntableComponent.h"
#include "PlaylistComponent.h"

//==============================================================================
class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer,
    public ChangeListener,
    public AngleChangeListener //custom listener class for angle changes
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint(Graphics&) override;
    void resized() override;
    void buttonClicked(Button*) override;
    void sliderValueChanged(Slider* slider) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void timerCallback() override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void angleChanged(float angleDifference) override;

    void loadTrack(const URL& trackURL, const String& title);

private:
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };

    Slider volSlider;
    Slider speedSlider;

    Label volSliderLabel;
    Label speedSliderLabel;

    FileChooser fChooser{ "Select a file..." };

    WaveformDisplay waveformDisplay;
    DJAudioPlayer* player;
    TurntableComponent turntable;
    TextEditor titleBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
