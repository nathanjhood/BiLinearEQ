/*
  ==============================================================================

    PluginWrapper.h
    Created: 8 May 2022 9:38:17pm
    Author:  StoneyDSP

  ==============================================================================
*/

#pragma once

#ifndef PLUGINWRAPPER_H_INCLUDED
#define PLUGINWRAPPER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Modules/BiLinearFilters.h"

class BiLinearEQAudioProcessor;

template <typename SampleType>
class ProcessWrapper
{
public:
    using APVTS = juce::AudioProcessorValueTreeState;
    //==========================================================================
    /** Constructor. */
    ProcessWrapper(BiLinearEQAudioProcessor& p, APVTS& apvts);

    //==========================================================================
    /** Initialises the processor. */
    void prepare();

    /** Resets the internal state variables of the processor. */
    void reset();

    //==========================================================================
    void process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages);

    //==========================================================================
    /** Updates the internal state variables of the processor. */
    void update();

private:
    //==========================================================================
    // This reference is provided as a quick way for the wrapper to
    // access the processor object that created it.
    BiLinearEQAudioProcessor& audioProcessor;
    APVTS& state;

    //==========================================================================
    /** Instantiate objects. */
    juce::dsp::ProcessSpec spec;
    juce::dsp::ProcessSpec& getSpec() { return spec; };

    juce::dsp::DryWetMixer<SampleType> mixer;
    BiLinearFilters<SampleType> hpFilter, lsFilter, hsFilter, lpFilter;
    juce::dsp::Gain<SampleType> output;

    //==========================================================================
    /** Parameter pointers. */
    juce::AudioParameterFloat* outputPtr { nullptr };
    juce::AudioParameterFloat* mixPtr { nullptr };
    juce::AudioParameterBool* bypassPtr { nullptr };
    juce::AudioParameterFloat* hpFreqPtr { nullptr };
    juce::AudioParameterFloat* lsFreqPtr { nullptr };
    juce::AudioParameterFloat* lsGainPtr { nullptr };
    juce::AudioParameterFloat* hsFreqPtr { nullptr };
    juce::AudioParameterFloat* hsGainPtr { nullptr };
    juce::AudioParameterFloat* lpFreqPtr { nullptr };

    //==========================================================================
    /** Init variables. */

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessWrapper)
};

#endif //  PLUGINWRAPPER_H_INCLUDED