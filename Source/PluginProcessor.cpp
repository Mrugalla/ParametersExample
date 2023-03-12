
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParametersExampleAudioProcessor::ParametersExampleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
	apvts(*this, nullptr, "Parameters", param::createParameterLayout()),
    params()
#endif
{
    for (auto i = 0; i < param::NumParams; ++i)
    {
		const auto pID = static_cast<param::PID>(i);
        const auto id = param::toID(pID);
        params[i] = apvts.getParameter(id);
    }
}

ParametersExampleAudioProcessor::~ParametersExampleAudioProcessor()
{
}

//==============================================================================
const juce::String ParametersExampleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParametersExampleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParametersExampleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParametersExampleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParametersExampleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParametersExampleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParametersExampleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParametersExampleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ParametersExampleAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParametersExampleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ParametersExampleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ParametersExampleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParametersExampleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ParametersExampleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
	auto numSamples = buffer.getNumSamples();
    if (numSamples == 0)
        return;
    {
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, numSamples);
    }
    
	const auto gainWetPID = static_cast<int>(param::PID::GainWet);
    const auto gainDbNorm = params[gainWetPID]->getValue();
    const auto gainDb = params[gainWetPID]->getNormalisableRange().convertFrom0to1(gainDbNorm);
    const auto gain = juce::Decibels::decibelsToGain(gainDb);
    buffer.applyGain(gain);
}

//==============================================================================
bool ParametersExampleAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ParametersExampleAudioProcessor::createEditor()
{
    return new ParametersExampleAudioProcessorEditor (*this);
}

//==============================================================================
void ParametersExampleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void ParametersExampleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametersExampleAudioProcessor();
}
