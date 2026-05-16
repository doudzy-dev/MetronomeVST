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
       parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
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
        elapsedSamples = 0;

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
    const double ppqPerSample =
        hostBpm / 60.0 / sampleRate;


    const int subdivisionChoice =
    static_cast<int>(parameters.getRawParameterValue("subdivision")->load());

    
    const int stepsPerBeat = getStepsPerBeatForMode(subdivisionChoice);
    const double subdivisionsPerBeat = static_cast<double>(stepsPerBeat); 
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const double currentPpq =
            ppqPosition + static_cast<double>(sample) * ppqPerSample;

        const int subdivision =
            static_cast<int>(std::floor(currentPpq * subdivisionsPerBeat));

        if (subdivision != lastSubdivision)
        {
            lastSubdivision = subdivision;

            const int beat = static_cast<int>(std::floor(currentPpq));

            currentBeatInBar = beat % 4;

            currentSubdivisionInBar =
                subdivision % static_cast<int>(4 * subdivisionsPerBeat);

            const int stepInBeat = subdivision % stepsPerBeat;

            const bool shouldClick =
                shouldTriggerPatternStep(subdivisionChoice, stepInBeat);

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
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "subdivision",
        "Subdivision",
        juce::StringArray { "1/4", "1/8", "1/16", "1/8T", "Gallop", "Reverse Gallop"},
        0
    ));
    return { params.begin(), params.end() };
}

bool MetronomeVSTAudioProcessor::shouldTriggerPatternStep(int subdivisionChoice,
                                                             int stepInBeat)
{
    switch (subdivisionChoice)
    {
        case 0: // 1/4
            return stepInBeat == 0;

        case 1: // 1/8
            return stepInBeat == 0 || stepInBeat == 1;

        case 2: // 1/16
            return true;

        case 3: // 1/8T
            return true;

        case 4: // Gallop: 1 e & a = X . X X
        {
            static constexpr bool pattern[4] = {
                true, false, true, true
            };

            return pattern[stepInBeat % 4];
        }
        case 5:
        {
            static constexpr bool pattern[4] = {
                true, true, true, false
            };

            return pattern[stepInBeat % 4];
        }
        default:
            return stepInBeat == 0;
    }
}

int MetronomeVSTAudioProcessor::getStepsPerBeatForMode(int subdivisionChoice)
{
    switch (subdivisionChoice)
    {
        case 0: return 1; // 1/4
        case 1: return 2; // 1/8
        case 2: return 4; // 1/16
        case 3: return 3; // 1/8T
        case 4: return 4; // Gallop
        case 5: return 4; // Gallop
        default: return 1;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MetronomeVSTAudioProcessor();
}