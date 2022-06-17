/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BiLinearEQAudioProcessor::BiLinearEQAudioProcessor() :
    AudioProcessor(BusesProperties().withInput("Input",     juce::AudioChannelSet::stereo(), true)
                                    .withOutput("Output",   juce::AudioChannelSet::stereo(), true)),
    apvts (*this, &undoManager, "Parameters", createParameterLayout())
{
}

BiLinearEQAudioProcessor::~BiLinearEQAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorParameter* BiLinearEQAudioProcessor::getBypassParameter() const
{
    return bypassPtr;
}

bool BiLinearEQAudioProcessor::isBypassed() const noexcept
{
    return bypassPtr->get() == true;
}

void BiLinearEQAudioProcessor::setBypassParameter(juce::AudioParameterBool* newBypass) noexcept
{
    if (bypassPtr != newBypass)
    {
        bypassPtr = newBypass;
        releaseResources();
        reset();
    }

}

bool BiLinearEQAudioProcessor::supportsDoublePrecisionProcessing() const
{
    return true;
}

juce::AudioProcessor::ProcessingPrecision BiLinearEQAudioProcessor::getProcessingPrecision() const noexcept
{
    return processingPrecision;
}

bool BiLinearEQAudioProcessor::isUsingDoublePrecision() const noexcept
{
    return processingPrecision == doublePrecision;
}

void BiLinearEQAudioProcessor::setProcessingPrecision(ProcessingPrecision newPrecision) noexcept
{
    // If you hit this assertion then you're trying to use double precision
    // processing on a processor which does not support it!
    jassert(newPrecision != doublePrecision || supportsDoublePrecisionProcessing());

    if (processingPrecision != newPrecision)
    {
        processingPrecision = newPrecision;
        releaseResources();
        reset();
    }
}

//==============================================================================
const juce::String BiLinearEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BiLinearEQAudioProcessor::acceptsMidi() const
{
    return false;
}

bool BiLinearEQAudioProcessor::producesMidi() const
{
    return false;
}

bool BiLinearEQAudioProcessor::isMidiEffect() const
{
    return false;
}

double BiLinearEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BiLinearEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BiLinearEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BiLinearEQAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String BiLinearEQAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void BiLinearEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void BiLinearEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..    

    getProcessingPrecision();

    processorFloat.prepare();
    processorDouble.prepare();
}

void BiLinearEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

    processorFloat.reset();
    processorDouble.reset();
}

void BiLinearEQAudioProcessor::reset()
{
    processorFloat.reset();
    processorDouble.reset();
}

void BiLinearEQAudioProcessor::numChannelsChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare();
    processorDouble.prepare();
}

void BiLinearEQAudioProcessor::numBusesChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare();
    processorDouble.prepare();
}

void BiLinearEQAudioProcessor::processorLayoutsChanged()
{
    processorFloat.reset();
    processorDouble.reset();
    processorFloat.prepare();
    processorDouble.prepare();
}

bool BiLinearEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

//==============================================================================
void BiLinearEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    getProcessingPrecision();

    if (bypassPtr->get() == false)
    {
        juce::ScopedNoDenormals noDenormals;

        processorFloat.process(buffer, midiMessages);
    }

    else
    {
        processBlockBypassed(buffer, midiMessages);
    }
}

void BiLinearEQAudioProcessor::processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    //setProcessingPrecision(static_cast<ProcessingPrecision>(precisionPtr->getIndex()));

    getProcessingPrecision();

    if (bypassPtr->get() == false)
    {
        juce::ScopedNoDenormals noDenormals;

        processorDouble.process(buffer, midiMessages);
    }

    else
    {
        processBlockBypassed(buffer, midiMessages);
    }
    
    // If you hit this assertion then either the caller called the double
    // precision version of processBlock on a processor which does not support it
    // (i.e. supportsDoublePrecisionProcessing() returns false), or the implementation
    // of the AudioProcessor forgot to override the double precision version of this method
    jassertfalse;

}

void BiLinearEQAudioProcessor::processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //setProcessingPrecision(static_cast<ProcessingPrecision>(precisionPtr->getIndex()));

    getProcessingPrecision();

    juce::ignoreUnused(buffer, midiMessages);
}

void BiLinearEQAudioProcessor::processBlockBypassed(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    //setProcessingPrecision(static_cast<ProcessingPrecision>(precisionPtr->getIndex()));

    getProcessingPrecision();

    juce::ignoreUnused(buffer, midiMessages);
}

//==============================================================================
bool BiLinearEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BiLinearEQAudioProcessor::createEditor()
{
    return new BiLinearEQAudioProcessorEditor(*this, getAPVTS(), undoManager );
}

juce::AudioProcessorValueTreeState::ParameterLayout BiLinearEQAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout params;

    Parameters::setParameterLayout(params);

    return params;
}

//==============================================================================
void BiLinearEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BiLinearEQAudioProcessor::getCurrentProgramStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BiLinearEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void BiLinearEQAudioProcessor::setCurrentProgramStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BiLinearEQAudioProcessor();
}
