/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NocturneDSPAudioProcessor::NocturneDSPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParams())
#endif
{
    loadBoost();

    loadProfile(0, BinaryData::model_clean_1302_210923_json);
    loadProfile(1, BinaryData::model_crunch_1758_210924_json);
    loadProfile(2, BinaryData::model_rhythm_1343_210921_json);
    /// TODO: replace with lead profile!
    loadProfile(3, BinaryData::model_clean_1302_210923_json);
}

NocturneDSPAudioProcessor::~NocturneDSPAudioProcessor()
{
}

//==============================================================================
const juce::String NocturneDSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NocturneDSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NocturneDSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NocturneDSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NocturneDSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NocturneDSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NocturneDSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NocturneDSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NocturneDSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void NocturneDSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NocturneDSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    cab.prepare(spec);
    cab.reset();
}

void NocturneDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NocturneDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void NocturneDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    updateParams();

    juce::dsp::AudioBlock<float> block = juce::dsp::AudioBlock<float>(buffer);
    
    input.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    /**
     * TODO: oversampling...
     */
    
    if(boostEnabled)
        boost.process(buffer.getReadPointer(0), buffer.getWritePointer(0), numSamples);
    
    gain.process(juce::dsp::ProcessContextReplacing<float>(block));

    channels[activeChannel].process(buffer.getReadPointer(0), buffer.getWritePointer(0), numSamples);

    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, numSamples);

    if(cabEnabled)
        cab.process(juce::dsp::ProcessContextReplacing<float>(block));

    volume.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool NocturneDSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NocturneDSPAudioProcessor::createEditor()
{
    return new NocturneDSPAudioProcessorEditor (*this);
}

//==============================================================================
void NocturneDSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Store an xml representation of our state.
    if (auto xmlState = state.copyState().createXml())
        copyXmlToBinary(*xmlState, destData);
}

void NocturneDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore our plug-in's state from the xml representation stored in the above method.
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NocturneDSPAudioProcessor();
}

void NocturneDSPAudioProcessor::loadImpulseResponse(const char *impulse, const int size)
{
    this->suspendProcessing(true);

    cab.loadImpulseResponse(
        impulse,
        size,
        juce::dsp::Convolution::Stereo::no,
        juce::dsp::Convolution::Trim::no,
        0
    );

    this->suspendProcessing(false);
}

void NocturneDSPAudioProcessor::loadProfile(int index, const char* binary)
{
    std::cout << "LOADING AMP MODEL: " << index << std::endl;
    
    this->suspendProcessing(true);

    channels[index].load_binary(binary);
    channels[index].reset();
    
    this->suspendProcessing(false);
}

juce::AudioProcessorValueTreeState::ParameterLayout NocturneDSPAudioProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Channel
    layout.add(std::make_unique<juce::AudioParameterInt>("CHANNEL", "Channel", 1, 4, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("BOOSTENABLED", "BoostEnabled", 0, 1, 0));

    // Cab
    layout.add(std::make_unique<juce::AudioParameterInt>("CAB", "Cab", 1, 2, 1));
    layout.add(std::make_unique<juce::AudioParameterInt>("CABENABLED", "CabEnabled", 0, 1, 1));

    // Input
    layout.add(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", juce::NormalisableRange<float>(-12.f, 12.f, .1f), DEFAULT_INPUT));
    
    // Gain
    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>(1.f, 3.f, .01f), DEFAULT_GAIN));

    // Volume
    layout.add(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", juce::NormalisableRange<float>(-12.f, 12.f, .1f), DEFAULT_VOLUME));
    
    return layout;
}

void NocturneDSPAudioProcessor::updateParams()
{
    // Input
    input.setGainDecibels(state.getRawParameterValue("INPUT")->load());

    // Boost
    boostEnabled = state.getRawParameterValue("BOOSTENABLED")->load() == 1;
    
    // Gain
    gain.setGainLinear(state.getRawParameterValue("GAIN")->load());
    
    // Channel
    const int selectedChannel = state.getRawParameterValue("CHANNEL")->load();
    if(activeChannel != selectedChannel - 1)
    {
        activeChannel = selectedChannel - 1;
        channels[activeChannel].reset();
    }
    
    // Cab
    cabEnabled = state.getRawParameterValue("CABENABLED")->load() == 1;
    
    // Volume
    volume.setGainDecibels(state.getRawParameterValue("VOLUME")->load());
}

void NocturneDSPAudioProcessor::loadBoost()
{
    this->suspendProcessing(true);
    
    std::cout << "LOADING BOOST MODEL..." << std::endl;

    boost.load_binary(BinaryData::model_boost_1202_210922_json);
    boost.reset();
    
    this->suspendProcessing(false);
}
