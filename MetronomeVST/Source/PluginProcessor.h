/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <atomic>
//==============================================================================
/**
*/
class MetronomeVSTAudioProcessor  : public juce::AudioProcessor
{
  public:
    //==============================================================================
    MetronomeVSTAudioProcessor();
    ~MetronomeVSTAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::atomic<bool> beatFlash { false };
    std::atomic<int> displayedBeat { 0 };
    int lastSubdivision = -1;
    int currentSubdivisionInBar = 0;

    std::atomic<double> elapsedSeconds { 0.0 };
    int64_t elapsedSamples = 0;
    bool wasPlaying = false;
  private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeVSTAudioProcessor)

    double sampleRate = 44100.0;
    //double bpm = 120.0;

    //int samplesUntilNextClick = 0;
    //int clickSamplesRemaining = 0;
    //int beatCounter = 0;
    //float clickPhase = 0.0f;

    int lastBeat = -1;
    int currentBeatInBar = 0;
    int clickSamplesRemaining = 0;
    float clickPhase = 0.0f;

    bool isPlaying = false;
    double hostBpm = 120.0;
    double ppqPosition = 0.0;
};
