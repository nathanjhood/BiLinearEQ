/*
  ==============================================================================

    PluginWrapper.cpp
    Created: 8 May 2022 9:38:17pm
    Author:  StoneyDSP

  ==============================================================================
*/

#include "PluginWrapper.h"
#include "PluginProcessor.h"

template <typename SampleType>
ProcessWrapper<SampleType>::ProcessWrapper(BiLinearEQAudioProcessor& p, APVTS& apvts) : audioProcessor(p), state(apvts)
{

    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    outputPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("outputID"));
    mixPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("mixID"));
    bypassPtr = dynamic_cast <juce::AudioParameterBool*> (state.getParameter("bypassID"));

    freqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("frequencyID"));
    gainPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("gainID"));
    typePtr = dynamic_cast <juce::AudioParameterChoice*> (state.getParameter("typeID"));
    transformPtr = dynamic_cast <juce::AudioParameterChoice*> (state.getParameter("transformID"));

    jassert(outputPtr != nullptr);
    jassert(mixPtr != nullptr);
    jassert(bypassPtr != nullptr);

    jassert(freqPtr != nullptr);
    jassert(gainPtr != nullptr);
    jassert(typePtr != nullptr);
    jassert(transformPtr != nullptr);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::prepare()
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    mixer.prepare(spec);
    filters.prepare(spec);
    output.prepare(spec);

    reset();
    update();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::reset()
{
    mixer.reset();
    filters.reset(static_cast<SampleType>(0.0));
    output.reset();
}

//==============================================================================
template <typename SampleType>
void ProcessWrapper<SampleType>::process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages)
{
    midiMessages.clear();

    update();

    juce::dsp::AudioBlock<SampleType> block(buffer);

    mixer.pushDrySamples(block);

    auto context = juce::dsp::ProcessContextReplacing(block);

    context.isBypassed = bypassPtr->get();

    filters.process(context);

    output.process(context);

    mixer.mixWetSamples(block);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::update()
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    mixer.setWetMixProportion(mixPtr->get() * 0.01f);

    filters.setFrequency(freqPtr->get());
    filters.setGain(gainPtr->get());
    filters.setFilterType(static_cast<FilterType>(typePtr->getIndex()));
    filters.setTransformType(static_cast<TransformationType>(transformPtr->getIndex()));

    output.setGainLinear(juce::Decibels::decibelsToGain(outputPtr->get()));
}

//==============================================================================
template class ProcessWrapper<float>;
template class ProcessWrapper<double>;