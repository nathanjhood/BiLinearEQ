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
    void prepare(juce::dsp::ProcessSpec& spec);

    /** Resets the internal state variables of the processor. */
    void reset();

    //==============================================================================
    /** Sets the length of the ramp used for smoothing parameter changes. */
    void setRampDurationSeconds(double newDurationSeconds) noexcept;

    /** Returns the ramp duration in seconds. */
    double getRampDurationSeconds() const noexcept;

    /** Returns true if the current value is currently being interpolated. */
    bool isSmoothing() const noexcept;

    //==========================================================================
    void process(juce::AudioBuffer<SampleType>& buffer, juce::MidiBuffer& midiMessages);

    //==========================================================================
    /** Updates the internal state variables of the processor. */
    void update();

    double rampDurationSeconds = 0.00005;

private:
    //==========================================================================
    // This reference is provided as a quick way for the wrapper to
    // access the processor object that created it.
    BiLinearEQAudioProcessor& audioProcessor;
    APVTS& state;

    //==========================================================================
    /** Instantiate objects. */
    juce::dsp::DryWetMixer<SampleType> mixer;
    juce::dsp::Gain<SampleType> output;

    using filter = BiLinearFilters<SampleType>;

    enum chainIndexes
    {
        highPass,
        lowShelf,
        highShelf,
        lowPass
    };

    juce::dsp::ProcessorChain<filter, filter, filter, filter> filterChain;

    //==========================================================================
    /** Parameter smoothers. */

    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> hpFrq;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> lpFrq;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> hsFrq;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> lsFrq;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear> hsLev;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear> lsLev;

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