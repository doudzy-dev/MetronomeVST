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

    bpmLabel.setText("BPM", juce::dontSendNotification);
    bpmLabel.setJustificationType(juce::Justification::centred);

    bpmSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 25);
    bpmSlider.setRange(40.0, 260.0, 1.0);

    //addAndMakeVisible(bpmLabel);
    //addAndMakeVisible(bpmSlider);

    bpmAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "bpm",
        bpmSlider
    );

    subdivisionLabel.setText("Subdivision", juce::dontSendNotification);
    subdivisionLabel.setJustificationType(juce::Justification::centred);

    subdivisionBox.addItem("1/4", 1);
    subdivisionBox.addItem("1/8", 2);
    subdivisionBox.addItem("1/16", 3);
    subdivisionBox.addItem("1/8T", 4);

    addAndMakeVisible(subdivisionLabel);
    addAndMakeVisible(subdivisionBox);

    subdivisionAttachment = std::make_unique<ComboBoxAttachment>(
        audioProcessor.parameters,
        "subdivision",
        subdivisionBox
    );

    timerLabel.setJustificationType(juce::Justification::centred);
    timerLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    timerLabel.setText("00:00", juce::dontSendNotification);

    addAndMakeVisible(timerLabel);

    setSize (300, 250);

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
}

void MetronomeVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    bpmLabel.setBounds(0, 105, getWidth(), 30);
    //bpmSlider.setBounds(75, 135, 150, 150);
    //subdivisionLabel.setBounds(0, 150, getWidth(), 25);
    subdivisionBox.setBounds(95, 150, 110, 30);
    timerLabel.setBounds(0, 190, getWidth(), 35);
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