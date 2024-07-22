/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse) 
               :player(_player), 
                waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volSliderLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(turntable);

    //setup for volSlider label
    volSliderLabel.setText("Volume", dontSendNotification);
    volSliderLabel.setJustificationType(Justification::centred);

    //setup for speedSlider label
    speedSliderLabel.setText("Speed", dontSendNotification);
    speedSliderLabel.setJustificationType(Justification::centred);

    //title box properties
    addAndMakeVisible(titleBox);
    titleBox.setReadOnly(true);
    titleBox.setCaretVisible(false);

    //font size
    Font boldFont(18.0f, Font::bold); //font height,style
    titleBox.setFont(boldFont);
    titleBox.setJustification(Justification::centred);

    //setup for btns
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    //setup for sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 100.0); //range cannot be 0 if not exception thrown

    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);

    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);

    getLookAndFeel().setColour(Slider::thumbColourId, Colours::whitesmoke);
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colours::darkgrey);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::rebeccapurple);
    startTimer(500);

    player->addChangeListener(this); //add change listener to the change broadcaster in djaudioplayer
    turntable.addAngleChangeListener(this);
}

DeckGUI::~DeckGUI()
{
    turntable.removeAngleChangeListener(this);
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1); //draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true); //draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 20; 
    int margin = 18;

    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    titleBox.setBounds(0, rowH * 2, getWidth(), rowH * 2);
    volSlider.setBounds(getWidth() / 2, rowH * 4, getWidth()/3, rowH * 5 - margin);
    volSliderLabel.setBounds(volSlider.getX(), volSlider.getBottom(), volSlider.getWidth(), 20);
    speedSlider.setBounds(getWidth()/5, rowH * 4, getWidth()/3, rowH * 5 - margin);
    speedSliderLabel.setBounds(speedSlider.getX(), speedSlider.getBottom(), speedSlider.getWidth(), 20);
    turntable.setBounds(0, rowH * 9, getWidth(), rowH * 8); //turntable takes up 12 rows
    waveformDisplay.setBounds(0, rowH * 17, getWidth(), rowH * 2.5);
    loadButton.setBounds(0, rowH * 19.25, getWidth(), rowH*1.5);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
    }
    if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            String title = chooser.getResult().getFileName(); //get file name for title
            titleBox.setText(title);
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());

    //update the rotation of the turntable.
    if (player->isPlaying())
    {
        turntable.rotationUpdate(9.0f);
    }

}

//change message from djaudioplayer sent here
void DeckGUI::changeListenerCallback(ChangeBroadcaster* source)
{
    //reset disc to original angle
    turntable.resetRotation();
}

//listener function for turntablecomponent angleDifference
void DeckGUI::angleChanged(float angleDifference)
{
    player->setPosByDiscRotation(angleDifference);
}
    
//this function will load the track to both player and waveformDisplay from the MainComponent
void DeckGUI::loadTrack(const URL& trackURL, const String& title)
{
    player->loadURL(trackURL);
    waveformDisplay.loadURL(trackURL);
    titleBox.setText(title);
}
