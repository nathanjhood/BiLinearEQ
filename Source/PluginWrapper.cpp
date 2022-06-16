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
    outputPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("outputID"));
    mixPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("mixID"));
    bypassPtr = dynamic_cast <juce::AudioParameterBool*> (state.getParameter("bypassID"));

    hpFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hpFrequencyID"));

    lsFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lsFrequencyID"));
    lsGainPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lsGainID"));

    hsFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hsFrequencyID"));
    hsGainPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("hsGainID"));

    lpFreqPtr = dynamic_cast <juce::AudioParameterFloat*> (state.getParameter("lpFrequencyID"));

    jassert(outputPtr != nullptr);
    jassert(mixPtr != nullptr);
    jassert(bypassPtr != nullptr);

    jassert(hpFreqPtr != nullptr);
    jassert(lpFreqPtr != nullptr);

    auto& hp = filterChain.template get<chainIndexes::highPass>();
    hp.setFrequency(hpFreqPtr->get());
    hp.setFilterType(FilterType::highPass);
    hp.setTransformType(TransformationType::directFormIItransposed);

    auto& lp = filterChain.template get<chainIndexes::lowPass>();
    lp.setFrequency(lpFreqPtr->get());
    lp.setFilterType(FilterType::lowPass);
    lp.setTransformType(TransformationType::directFormIItransposed);

    auto& ls = filterChain.template get<chainIndexes::lowShelf>();
    ls.setFrequency(lsFreqPtr->get());
    ls.setGain(lsGainPtr->get());
    ls.setFilterType(FilterType::lowShelf);
    ls.setTransformType(TransformationType::directFormIItransposed);
    
    auto& hs = filterChain.template get<chainIndexes::highShelf>();
    hs.setFrequency(hsFreqPtr->get());
    hs.setGain(lsGainPtr->get());
    hs.setFilterType(FilterType::lowShelf);
    hs.setTransformType(TransformationType::directFormIItransposed);
}

//==============================================================================
template <typename SampleType>
void ProcessWrapper<SampleType>::setRampDurationSeconds(double newDurationSeconds) noexcept
{
    if (rampDurationSeconds != newDurationSeconds)
    {
        rampDurationSeconds = newDurationSeconds;
        reset();
    }
}

template <typename SampleType>
double ProcessWrapper<SampleType>::getRampDurationSeconds() const noexcept
{
    return rampDurationSeconds;
}

template <typename SampleType>
bool ProcessWrapper<SampleType>::isSmoothing() const noexcept
{
    bool isSmoothing = hpFrq.isSmoothing() || lpFrq.isSmoothing() || lsLev.isSmoothing() || hsLev.isSmoothing();

    return isSmoothing;
}

template <typename SampleType>
void ProcessWrapper<SampleType>::prepare(juce::dsp::ProcessSpec& spec)
{
    spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();

    mixer.prepare(spec);

    auto& lp = filterChain.template get<chainIndexes::lowPass>();
    lp.prepare(spec);

    auto& hp = filterChain.template get<chainIndexes::highPass>();
    hp.prepare(spec);

    auto& ls = filterChain.template get<chainIndexes::lowShelf>();
    ls.prepare(spec);

    auto& hs = filterChain.template get<chainIndexes::highShelf>();
    hs.prepare(spec);

    output.prepare(spec);

    reset();
    update();
}

template <typename SampleType>
void ProcessWrapper<SampleType>::reset()
{
    mixer.reset();
    filterChain.reset();
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

    filterChain.process(context);

    output.process(context);

    mixer.mixWetSamples(block);
}

template <typename SampleType>
void ProcessWrapper<SampleType>::update()
{
    /*spec.sampleRate = audioProcessor.getSampleRate();
    spec.maximumBlockSize = audioProcessor.getBlockSize();
    spec.numChannels = audioProcessor.getTotalNumInputChannels();*/

    mixer.setWetMixProportion(mixPtr->get() * 0.01f);

    auto& hp = filterChain.template get<chainIndexes::highPass>();
    hp.setFrequency(hpFreqPtr->get());

    auto& lp = filterChain.template get<chainIndexes::lowPass>();
    lp.setFrequency(lpFreqPtr->get());

    auto& ls = filterChain.template get<chainIndexes::lowShelf>();
    ls.setFrequency(lsFreqPtr->get());
    ls.setGain(lsGainPtr->get());

    auto& hs = filterChain.template get<chainIndexes::highShelf>();
    hs.setFrequency(hsFreqPtr->get());
    hs.setGain(lsGainPtr->get());

    output.setGainLinear(juce::Decibels::decibelsToGain(outputPtr->get()));
}

//==============================================================================
template class ProcessWrapper<float>;
template class ProcessWrapper<double>;