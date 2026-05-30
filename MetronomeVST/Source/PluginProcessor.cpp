/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include <cmath>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetronomeVSTAudioProcessor::MetronomeVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       parameters (*this, nullptr, "PARAMETERS", createParameterLayout())//,
       //apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

MetronomeVSTAudioProcessor::~MetronomeVSTAudioProcessor()
{
}

//==============================================================================
const juce::String MetronomeVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MetronomeVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MetronomeVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MetronomeVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MetronomeVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MetronomeVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MetronomeVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MetronomeVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MetronomeVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void MetronomeVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MetronomeVSTAudioProcessor::prepareToPlay (double sr, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // sampleRate = sr;
    //bpm = 120.0;

    sampleRate = sr;

    lastBeat = -1;
    currentBeatInBar = 0;

    clickSamplesRemaining = 0;
    clickPhase = 0.0f;
    lastSubdivision = -1;
    currentSubdivisionInBar = 0;

    elapsedSamples = 0;
    wasPlaying = false;
    elapsedSeconds.store(0.0);

    internalPpqPosition = 0.0;
}

void MetronomeVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MetronomeVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MetronomeVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    if (auto* playHead = getPlayHead())
    {
        if (auto position = playHead->getPosition())
        {
            if (position->getBpm().hasValue())
                hostBpm = *position->getBpm();

            isPlaying = position->getIsPlaying();

            if (position->getPpqPosition().hasValue())
                ppqPosition = *position->getPpqPosition();
        }
    }

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    const int clickLengthSamples = static_cast<int>(0.03 * sampleRate);
    if (isPlaying && !wasPlaying)
    {
        elapsedSamples = 0;
        internalPpqPosition = 0.0;
        lastBeat = -1;
        lastSubdivision = -1;
    }

    wasPlaying = isPlaying;
    if (!isPlaying)
    {
        lastBeat = -1;
        lastSubdivision = -1;
        clickSamplesRemaining = 0;
        elapsedSeconds.store(0.0);
        return;
    }
    elapsedSamples += numSamples;
    elapsedSeconds.store(static_cast<double>(elapsedSamples) / sampleRate);

    const double ppqPerSample = hostBpm / 60.0 / sampleRate;

    
//
    //const auto pattern = getPatternForMode(subdivisionChoice);
//
    //const int stepsPerBeat = pattern.stepsPerBeat;
    //const double subdivisionsPerBeat = static_cast<double>(stepsPerBeat);
    //const int beatsPerBar = static_cast<int>(parameters.getRawParameterValue("beatsPerBar")->load());
    

    /*-----------------------------------------------------------------------------------------------*/

    const int subdivisionChoice =
    static_cast<int>(parameters.getRawParameterValue("division")->load());

    const int beatsPerBar =
    static_cast<int>(parameters.getRawParameterValue("beatsPerBar")->load());

    const auto pattern = getPatternForMode(subdivisionChoice);

    const int stepsPerBeat = pattern.stepsPerBeat;
    
    const double subdivisionsPerBeat = static_cast<double>(stepsPerBeat);
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const double currentPpq =
            internalPpqPosition + static_cast<double>(sample) * ppqPerSample;

        const int subdivision =
            static_cast<int>(std::floor(currentPpq * subdivisionsPerBeat));

        if (subdivision != lastSubdivision)
        {
            lastSubdivision = subdivision;

            const int beat = static_cast<int>(std::floor(currentPpq));

            currentBeatInBar = beat % beatsPerBar;

            currentSubdivisionInBar = subdivision % static_cast<int>(beatsPerBar * subdivisionsPerBeat);

            const int stepInPattern =
                subdivision % static_cast<int>(pattern.steps.size());

            const bool shouldClick =
                pattern.steps[stepInPattern];

            if (shouldClick)
            {
                displayedBeat.store(currentBeatInBar);
                beatFlash.store(true);

                clickSamplesRemaining = clickLengthSamples;
                clickPhase = 0.0f;
            }
        }

        float click = 0.0f;

        if (clickSamplesRemaining > 0)
        {
            //const bool isAccent = currentBeatInBar == 0;
            const bool isDownbeat = currentBeatInBar == 0 && currentSubdivisionInBar == 0;
            const bool isBeat = currentSubdivisionInBar % static_cast<int>(subdivisionsPerBeat) == 0;
            
            //const float frequency = isAccent ? 1800.0f : 1000.0f;
            //const float amplitude = isAccent ? 0.8f : 0.45f;
            const float frequency = isDownbeat ? 1800.0f : (isBeat ? 1200.0f : 800.0f);
            const float amplitude = isDownbeat ? 0.8f : (isBeat ? 0.5f : 0.25f);
            
            click = std::sin(clickPhase) * amplitude;

            clickPhase += juce::MathConstants<float>::twoPi
                        * frequency
                        / static_cast<float>(sampleRate);

            const float fade =
                static_cast<float>(clickSamplesRemaining)
                / static_cast<float>(clickLengthSamples);

            click *= fade;

            --clickSamplesRemaining;
        }

        for (int channel = 0; channel < numChannels; ++channel)
            buffer.setSample(channel, sample, click);
    }
    internalPpqPosition += static_cast<double>(numSamples) * ppqPerSample;
}

//==============================================================================
bool MetronomeVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MetronomeVSTAudioProcessor::createEditor()
{
    return new MetronomeVSTAudioProcessorEditor (*this);
}

//==============================================================================
void MetronomeVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MetronomeVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout

MetronomeVSTAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "bpm",
        "BPM",
        juce::NormalisableRange<float>(40.0f, 260.0f, 1.0f),
        120.0f
    ));
    //params.push_back(std::make_unique<juce::AudioParameterChoice>(
    //    "subdivision",
    //    "Subdivision",
    //    juce::StringArray { "1/4", "1/8", "1/16", "1/8T", "Gallop", "Reverse Gallop"},
    //    0
    //));
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "beatsPerBar",
        "Beats Per Bar",
        1,
        12,
        4
    ));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("division", 1),
        "Division",
        juce::StringArray { "1/4", "1/8", "1/16", "1/8T", "Gallop", "Reverse Gallop","Sextolet"},
        0
    ));
    return { params.begin(), params.end() };
}

MetronomeVSTAudioProcessor::RhythmPattern

MetronomeVSTAudioProcessor::getPatternForMode(int subdivisionChoice)
{
    switch (subdivisionChoice)
    {
        case 0: // 1/4
            return { 1, { true } };
        case 1: // 1/8
            return { 2, { true, true } };
        case 2: // 1/16
            return { 4, { true, true, true, true } };
        case 3: // 1/8T
            return { 3, { true, true, true } };
        case 4: // Gallop: X . X X
            return { 4, { true, false, true, true } };
        case 5: // Reverse gallop: X X . X
            return { 4, { true, true, false, true } };
        case 6: // 1/8T
            return { 6, { true, true, true, true, true, true, } };
        //case 6: // Pattern 5 steps
        //    return { 4, { true, false, true, false, true } };
        //case 7: // Pattern 7 steps
        //    return { 4, { true, false, true, true, false, true, false } };
        default:
            return { 1, { true } };
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MetronomeVSTAudioProcessor();
}