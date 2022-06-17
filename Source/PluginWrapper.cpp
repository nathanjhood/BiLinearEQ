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
ProcessWrapper<SampleType>::ProcessWrapper(BiLinearEQAudioProcessor& p, APVTS& apvts) : spec(), audioProcessor(p), state(apvts)
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    bypassPtr = dynamic_cast <juce::AudioParameterBool*> (state.getParameter("bypassID"));
    precisionPtr = dynamic_cast <juce::AudioParameterChoice*> (state.getParameter("precisionID"));
    outputPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("outputID"));
    drywetPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("mixID"));
    hpFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hpFrequencyID"));
    lsFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lsFrequencyID"));
    lsGainPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lsGainID"));
    hsFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hsFrequencyID"));
    hsGainPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hsGainID"));
    lpFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lpFrequencyID"));

    jassert(bypassPtr != nullptr);
    jassert(precisionPtr != nullptr);
    jassert(outputPtr != nullptr);
    jassert(drywetPtr != nullptr);
    jassert(hpFreqPtr != nullptr);
    jassert(lsFreqPtr != nullptr);
    jassert(lsGainPtr != nullptr);
    jassert(hsFreqPtr != nullptr);
    jassert(hsGainPtr != nullptr);
    jassert(lpFreqPtr != nullptr);

    hpFilter.setFilterType(FilterType::highPass);
    lsFilter.setFilterType(FilterType::lowShelf);
    hsFilter.setFilterType(FilterType::highShelf);
    lpFilter.setFilterType(FilterType::lowPass);

    hpFilter.setTransformType(TransformationType::directFormIItransposed);
    lsFilter.setTransformType(TransformationType::directFormIItransposed);
    hsFilter.setTransformType(TransformationType::directFormIItransposed);
    lpFilter.setTransformType(TransformationType::directFormIItransposed);
}

//==============================================================================
template <typename SampleType>
void ProcessWrapper<SampleType>::prepare()
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    mixer.prepare(spec);

    hpFilter.prepare(spec);
    lsFilter.prepare(spec);
    hsFilter.prepare(spec);
    lpFilter.prepare(spec);

    output.prepare(spec);

    reset();
    update();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::reset()
{
    mixer.reset();
    hpFilter.reset(static_cast<SampleType>(0.0));
    lsFilter.reset(static_cast<SampleType>(0.0));
    hsFilter.reset(static_cast<SampleType>(0.0));
    lpFilter.reset(static_cast<SampleType>(0.0));
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

    hpFilter.process(context);
    lsFilter.process(context);
    hsFilter.process(context);
    lpFilter.process(context);
    output.process(context);

    mixer.mixWetSamples(block);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::update()
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    audioProcessor.setBypassParameter(bypassPtr);
    audioProcessor.setProcessingPrecision(static_cast<juce::AudioProcessor::ProcessingPrecision>(precisionPtr->getIndex()));

    mixer.setWetMixProportion(drywetPtr->get() * 0.01f);

    hpFilter.setFrequency(hpFreqPtr->get());
    lsFilter.setFrequency(lsFreqPtr->get());
    hsFilter.setFrequency(hsFreqPtr->get());
    lpFilter.setFrequency(lpFreqPtr->get());

    lsFilter.setGain(lsGainPtr->get());
    hsFilter.setGain(hsGainPtr->get());

    output.setGainLinear(juce::Decibels::decibelsToGain(outputPtr->get()));
}

//==============================================================================
template class ProcessWrapper<float>;
template class ProcessWrapper<double>;