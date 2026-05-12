/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MetronomeVSTAudioProcessorEditor  : public juce::AudioProcessorEditor,private juce::Timer
{
public:
    MetronomeVSTAudioProcessorEditor (MetronomeVSTAudioProcessor&);
    ~MetronomeVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider bpmSlider;
    juce::Label bpmLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> bpmAttachment;

    void timerCallback() override;

    bool ledOn = false;
    int ledCounter = 0;
    int currentDisplayedBeat = 0;

    juce::ComboBox subdivisionBox;
    juce::Label subdivisionLabel;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    std::unique_ptr<ComboBoxAttachment> subdivisionAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MetronomeVSTAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeVSTAudioProcessorEditor)
};
