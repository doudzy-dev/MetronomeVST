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

    addAndMakeVisible(bpmLabel);
    addAndMakeVisible(bpmSlider);

    bpmAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters,
        "bpm",
        bpmSlider
    );
    setSize (300, 220);
}

MetronomeVSTAudioProcessorEditor::~MetronomeVSTAudioProcessorEditor()
{
}

//==============================================================================
void MetronomeVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MetronomeVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    bpmLabel.setBounds(0, 20, getWidth(), 30);
    bpmSlider.setBounds(75, 55, 150, 150);
}
