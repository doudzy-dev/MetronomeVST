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
    void paintButtons(int currentActive);
    void paintButtons(juce::ImageButton button);
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

    juce::Label timerLabel;


    juce::Slider beatsPerBarSlider;
    juce::Label beatsPerBarLabel;

    std::unique_ptr<SliderAttachment> beatsPerBarAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MetronomeVSTAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeVSTAudioProcessorEditor)

    juce::ImageButton noireButton;
    juce::ImageButton crochesButton;
    juce::ImageButton dcrochesButton;
    juce::ImageButton trioletButton;
    juce::ImageButton sextoletButton;
    juce::ImageButton gallopButton;
    juce::ImageButton rgallopButton;

    juce::Image noireImage;
    juce::Image crochesImage;
    juce::Image dcrochesImage;
    juce::Image trioletImage;
    juce::Image sextoletImage;
    juce::Image gallopImage;
    juce::Image rgallopImage;


};
