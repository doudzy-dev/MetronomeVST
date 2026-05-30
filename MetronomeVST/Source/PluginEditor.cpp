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
    timerLabel.setFont(juce::Font(24.0f, juce::Font::bold));
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

    noireNormal = juce::ImageCache::getFromMemory(BinaryData::noire_png,
        BinaryData::noire_pngSize);
    crochesNormal = juce::ImageCache::getFromMemory(BinaryData::croches_png,
        BinaryData::croches_pngSize);
    dcrochesNormal = juce::ImageCache::getFromMemory(BinaryData::doubleCroches_png,
        BinaryData::doubleCroches_pngSize);
    trioletNormal = juce::ImageCache::getFromMemory(BinaryData::triolet_png,
        BinaryData::triolet_pngSize);                                      
    gallopNormal = juce::ImageCache::getFromMemory(BinaryData::galop_png,
        BinaryData::galop_pngSize);
    rgallopNormal = juce::ImageCache::getFromMemory(BinaryData::rgalop_png,
        BinaryData::rgalop_pngSize);                                      
    sextoletNormal = juce::ImageCache::getFromMemory(BinaryData::sextolet_png,
        BinaryData::sextolet_pngSize);
        
    noireButton.setImages(
        false, true, true,
        noireNormal, 1.0f, juce::Colours::white,
        noireNormal, 0.8f, juce::Colours::white,
        noireNormal, 0.6f, juce::Colours::white
    );
    crochesButton.setImages(
        false, true, true,
        crochesNormal, 1.0f, juce::Colours::white,
        crochesNormal, 0.8f, juce::Colours::white,
        crochesNormal, 0.6f, juce::Colours::white
    );
    dcrochesButton.setImages(
        false, true, true,
        dcrochesNormal, 1.0f, juce::Colours::white,
        dcrochesNormal, 0.8f, juce::Colours::white,
        dcrochesNormal, 0.6f, juce::Colours::white
    );
    trioletButton.setImages(
        false, true, true,
        trioletNormal, 1.0f, juce::Colours::white,
        trioletNormal, 0.8f, juce::Colours::white,
        trioletNormal, 0.6f, juce::Colours::white
    );
    gallopButton.setImages(
        false, true, true,
        gallopNormal, 1.0f, juce::Colours::white,
        gallopNormal, 0.8f, juce::Colours::white,
        gallopNormal, 0.6f, juce::Colours::white
    );
    rgallopButton.setImages(
        false, true, true,
        rgallopNormal, 1.0f, juce::Colours::white,
        rgallopNormal, 0.8f, juce::Colours::white,
        rgallopNormal, 0.6f, juce::Colours::white
    );
    sextoletButton.setImages(
        false, true, true,
        sextoletNormal, 1.0f, juce::Colours::white,
        sextoletNormal, 0.8f, juce::Colours::white,
        sextoletNormal, 0.6f, juce::Colours::white
    );
    
    
    
    auto* param = dynamic_cast<juce::AudioParameterChoice*>(
        audioProcessor.parameters.getParameter("division")
    );
    
    noireButton.onClick = [this, param]()
    {
        bpmLabel.setText("Noire", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 0;
        param->endChangeGesture();
    };
    
    crochesButton.onClick = [this, param]()
    {
        bpmLabel.setText("Croches", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 1;
        param->endChangeGesture();
    };
    
    dcrochesButton.onClick = [this, param]()
    {
        bpmLabel.setText("Doubles Croches", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 2;
        param->endChangeGesture();
    };
    
    trioletButton.onClick = [this, param]()
    {
        bpmLabel.setText("Triolet", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 3;
        param->endChangeGesture();
    };
    

    gallopButton.onClick = [this, param]()
    {
        bpmLabel.setText("Gallop", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 4;
        param->endChangeGesture();
    };
    
    rgallopButton.onClick = [this, param]()
    {
        bpmLabel.setText("Reverse Gallop", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 5;
        param->endChangeGesture();
    };
    
    sextoletButton.onClick = [this, param]()
    {
        bpmLabel.setText("Sextolet", juce::dontSendNotification);
        param->beginChangeGesture();
        *param = 6;
        param->endChangeGesture();
    };

    addAndMakeVisible(bpmLabel);
    
    addAndMakeVisible(timerLabel);
    
    addAndMakeVisible(beatsPerBarLabel);
    addAndMakeVisible(beatsPerBarSlider);
    
    addAndMakeVisible(noireButton);
    addAndMakeVisible(crochesButton);
    addAndMakeVisible(trioletButton);
    addAndMakeVisible(dcrochesButton);
    addAndMakeVisible(gallopButton);
    addAndMakeVisible(rgallopButton);
    addAndMakeVisible(sextoletButton);
    
    setSize (300, 400);
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


    auto ledBounds = juce::Rectangle<float>(110.0f, 20.0f, 80.0f, 80.0f);

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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    bpmLabel.setBounds(0, 105, getWidth(), 30);
    //bpmSlider.setBounds(75, 135, 150, 150);
    //subdivisionLabel.setBounds(0, 150, getWidth(), 25);
    subdivisionBox.setBounds(95, 150, 110, 30);
    
    beatsPerBarLabel.setBounds(0, 295, getWidth(), 25);
    beatsPerBarSlider.setBounds(40, 325, 220, 30);
    
    beatsPerBarLabel.setBounds(0, 190, getWidth(), 25);
    beatsPerBarSlider.setBounds(40, 220, 220, 30);
    
    timerLabel.setBounds(0, 235, getWidth(), 35);

    noireButton.setBounds(10, 300, 100, 50);
    crochesButton.setBounds(60, 300, 100, 50);
    trioletButton.setBounds(120, 300, 100, 50);
    dcrochesButton.setBounds(180, 300, 100, 50);
    gallopButton.setBounds(10, 350, 100, 50);
    rgallopButton.setBounds(70, 350, 100, 50);
    sextoletButton.setBounds(130, 350, 100, 50);
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