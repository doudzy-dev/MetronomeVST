/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetronomeVSTAudioProcessorEditor::MetronomeVSTAudioProcessorEditor (MetronomeVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    
 
    //bpmSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);
    //bpmSlider.setRange(40.0, 260.0, 1.0);
    //
    //addAndMakeVisible(bpmSlider);
    //bpmAttachment = std::make_unique<SliderAttachment>(
    //    audioProcessor.parameters,
    //    "bpm",
    //    bpmSlider
    //);
    //subdivisionLabel.setText("Subdivision", juce::dontSendNotification);
    //subdivisionLabel.setJustificationType(juce::Justification::centred);
    //subdivisionBox.addItem("1/4", 1);
    //subdivisionBox.addItem("1/8", 2);
    //subdivisionBox.addItem("1/16", 3);
    //subdivisionBox.addItem("1/8T", 4);
    //subdivisionBox.addItem("Gallop", 5);
    //subdivisionBox.addItem("Reverse Gallop", 6);
    //addAndMakeVisible(subdivisionLabel);
    //addAndMakeVisible(subdivisionBox);

    //subdivisionAttachment = std::make_unique<ComboBoxAttachment>(
    //    audioProcessor.parameters,
    //    "subdivision",
    //    subdivisionBox
    //);

    timerLabel.setJustificationType(juce::Justification::centred);
    timerLabel.setFont(juce::Font(40.0f, juce::Font::bold));
    timerLabel.setText("00:00", juce::dontSendNotification);

    beatsPerBarLabel.setText("Beats / Bar", juce::dontSendNotification);
    beatsPerBarLabel.setJustificationType(juce::Justification::centred);
    
    beatsPerBarSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    beatsPerBarSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 25);
    beatsPerBarSlider.setRange(1, 12, 1);
    
    
    beatsPerBarAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "beatsPerBar",
        beatsPerBarSlider
    );

    bpmLabel.setText("Noire", juce::dontSendNotification);
    bpmLabel.setJustificationType(juce::Justification::centred);

    noireImage = juce::ImageCache::getFromMemory(BinaryData::noire_png,
        BinaryData::noire_pngSize);
    crochesImage = juce::ImageCache::getFromMemory(BinaryData::croches_png,
        BinaryData::croches_pngSize);
    dcrochesImage = juce::ImageCache::getFromMemory(BinaryData::doubleCroches_png,
        BinaryData::doubleCroches_pngSize);
    trioletImage = juce::ImageCache::getFromMemory(BinaryData::triolet_png,
        BinaryData::triolet_pngSize);                                      
    gallopImage = juce::ImageCache::getFromMemory(BinaryData::galop_png,
        BinaryData::galop_pngSize);
    rgallopImage = juce::ImageCache::getFromMemory(BinaryData::rgalop_png,
        BinaryData::rgalop_pngSize);                                      
    sextoletImage = juce::ImageCache::getFromMemory(BinaryData::sextolet_png,
        BinaryData::sextolet_pngSize);
        
    map_noteImages = { {"noire",noireImage}, {"croches",crochesImage}, {"triolet",trioletImage}, {"dcroches",dcrochesImage}, 
        {"gallop",gallopImage}, {"rgallop",rgallopImage}, {"sextolet",sextoletImage} };

    map_imageButtons = { {"noire",&noireButton}, {"croches",&crochesButton}, { "triolet",&trioletButton }, { "dcroches",&dcrochesButton },
        {"gallop",&gallopButton}, {"rgallop",&rgallopButton}, {"sextolet",&sextoletButton} };

    for (const auto& image : map_imageButtons)
    {
        image.second->setSize(1024, 640);
    }
    
    paintButtons("noire");
    
    
    auto* param = dynamic_cast<juce::AudioParameterChoice*>(
        audioProcessor.parameters.getParameter("division")
    );
    
    noireButton.onClick = [this, param]()
    {
        paintButtons("noire");
        bpmLabel.setText("Noire", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 0;
        param->endChangeGesture();
    };
    
    crochesButton.onClick = [this, param]()
    {
        paintButtons("croches");
        bpmLabel.setText("Croches", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 1;
        param->endChangeGesture();
    };
    
    dcrochesButton.onClick = [this, param]()
    {
        paintButtons("dcroches");
        bpmLabel.setText("Doubles Croches", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 2;
        param->endChangeGesture();
    };
    
    trioletButton.onClick = [this, param]()
    {
        paintButtons("triolet");
        bpmLabel.setText("Triolet", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 3;
        param->endChangeGesture();
    };
    

    gallopButton.onClick = [this, param]()
    {
        paintButtons("gallop");
        bpmLabel.setText("Gallop", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 4;
        param->endChangeGesture();
    };
    
    rgallopButton.onClick = [this, param]()
    {
        paintButtons("rgallop");
        bpmLabel.setText("Reverse Gallop", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 5;
        param->endChangeGesture();
    };
    
    sextoletButton.onClick = [this, param]()
    {
        paintButtons("sextolet");
        bpmLabel.setText("Sextolet", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 6;
        param->endChangeGesture();
    };

    addAndMakeVisible(bpmLabel);
    
    addAndMakeVisible(timerLabel);
    
    addAndMakeVisible(beatsPerBarLabel);
    addAndMakeVisible(beatsPerBarSlider);
    
    for (const auto& button : map_imageButtons)
    {
        addAndMakeVisible(button.second);
    }

    //, { "triolet",trioletButton }, { "dcroches",dcrochesButton }, {"gallop",gallopButton}, {"rgallop",rgallopButton}, {"sextolet",sextoletButton} };
    setSize(535, 455);
    startTimerHz(60);
}

MetronomeVSTAudioProcessorEditor::~MetronomeVSTAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void MetronomeVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.fillAll(juce::Colours::black);

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);


    auto ledBounds = juce::Rectangle<float>(225.0f, 20.0f, 80.0f, 80.0f);

    const bool isAccent = currentDisplayedBeat == 0;

    //auto ledColour = ledOn
    //    ? (isAccent ? juce::Colours::red : juce::Colours::orange)
    //    : juce::Colours::darkgrey;

    auto ledColour = juce::Colours::darkgrey;

    g.setColour(ledColour);
    g.fillEllipse(ledBounds);

    g.setColour(juce::Colours::white);
    g.drawEllipse(ledBounds, 2.0f);

    g.setFont(24.0f);
    g.drawText(
        juce::String(currentDisplayedBeat + 1),
        ledBounds.toNearestInt(),
        juce::Justification::centred
    );
    juce::String(currentDisplayedBeat + 1);
}
//fake
void MetronomeVSTAudioProcessorEditor::resized()
{
    bpmLabel.setBounds(0, 105, getWidth(), 30);
    subdivisionBox.setBounds(95, 150, 110, 30);
    
    timerLabel.setBounds(0, 150, getWidth(), 40);

    beatsPerBarLabel.setBounds(0, 200, getWidth(), 25);
    beatsPerBarSlider.setBounds(160, 220, 220, 30);
    
    noireButton.setBounds(10, 250, 128, 128);
    crochesButton.setBounds(140, 250, 128, 128);
    trioletButton.setBounds(270, 250, 128, 128);
    dcrochesButton.setBounds(400, 250, 128, 128);
    gallopButton.setBounds(10, 340, 128, 128);
    rgallopButton.setBounds(140, 340, 128, 128);
    sextoletButton.setBounds(270, 340, 128, 128);
}

void MetronomeVSTAudioProcessorEditor::paintButtons(std::string currentActive)
{
    for (const auto& button : map_imageButtons)
    {
        if(button.first == currentActive)
        { 
            button.second->setImages(
                false, true, true,
                map_noteImages[button.first], 1.0f, juce::Colours::red,
                map_noteImages[button.first], 0.8f, juce::Colours::red,
                map_noteImages[button.first], 0.6f, juce::Colours::red
            );
        }
        else
        {
            button.second->setImages(
                false, true, true,
                map_noteImages[button.first], 1.0f, juce::Colours::white,
                map_noteImages[button.first], 0.8f, juce::Colours::white,
                map_noteImages[button.first], 0.6f, juce::Colours::white
            );
        }
        //button.second->setSize(1024, 640);
    }
}


void MetronomeVSTAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.beatFlash.exchange(false))
    {
        ledOn = true;
        ledCounter = 6;
        currentDisplayedBeat = audioProcessor.displayedBeat.load();
    }

    if (ledCounter > 0)
    {
        --ledCounter;
    }
    else
    {
        ledOn = false;
    }
    const int totalSeconds =
        static_cast<int>(audioProcessor.elapsedSeconds.load());

    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;

    timerLabel.setText(
        juce::String::formatted("%02d:%02d", minutes, seconds),
        juce::dontSendNotification
    );
    repaint();
}