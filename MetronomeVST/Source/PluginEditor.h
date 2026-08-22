/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <map>
#include <string>
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
    void paintButtons(std::string currentActive);

    void timerCallback() override;
    
    bool ledOn = false;
    int ledCounter = 0;
    int currentDisplayedBeat = 0;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
  private:
    MetronomeVSTAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeVSTAudioProcessorEditor)
    
    juce::Label timerLabel;
    juce::Slider beatsPerBarSlider;
    juce::Label beatsPerBarLabel;
    juce::ComboBox subdivisionBox;
    juce::Label subdivisionLabel;
    juce::Slider bpmSlider;
    juce::Label bpmLabel;
    
    std::unique_ptr<ComboBoxAttachment> subdivisionAttachment;
    std::unique_ptr<SliderAttachment> beatsPerBarAttachment;
    std::unique_ptr<SliderAttachment> bpmAttachment;
    

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

    std::map<std::string,juce::Image> map_noteImages;
    std::map<std::string,juce::ImageButton*> map_imageButtons;    
};
