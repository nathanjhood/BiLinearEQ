/*
  ==============================================================================

    PluginParameters.cpp
    Created: 29 May 2022 7:58:00pm
    Author:  Nathan J. Hood

  ==============================================================================
*/

#include "PluginParameters.h"
#include "PluginProcessor.h"

Parameters::Parameters(BiLinearEQAudioProcessor& p, APVTS& apvts) : audioProcessor(p), state(apvts)
{
}

void Parameters::setParameterLayout(Params& params)
{
    const auto dBMax = juce::Decibels::gainToDecibels(32.0f);
    const auto dBMin = juce::Decibels::gainToDecibels(0.03125f);
    const auto dBOut = juce::Decibels::gainToDecibels(0.5f, -120.0f) * 20.0f;

    const auto freqRange = juce::NormalisableRange<float>(20.00f, 20000.00f, 0.001f, 00.198894f);
    const auto gainRange = juce::NormalisableRange<float>(dBMin, dBMax, 0.01f, 1.00f);
    const auto mixRange = juce::NormalisableRange<float>(00.00f, 100.00f, 0.01f, 1.00f);
    const auto outputRange = juce::NormalisableRange<float>(dBOut, dBMax, 0.01f, 1.00f);

    const auto decibels = juce::String { ( "dB" ) };
    const auto frequency = juce::String { ( "Hz" ) };
    const auto percentage = juce::String { ( "%"  ) };
    const auto inOut = juce::String { ("IO") };

    //const auto pString = juce::StringArray{ "Floats", "Doubles" };

    const auto genParam = juce::AudioProcessorParameter::Category::genericParameter;;
    const auto inMeter = juce::AudioProcessorParameter::Category::inputMeter;
    const auto outParam = juce::AudioProcessorParameter::Category::outputGain;
    const auto outMeter = juce::AudioProcessorParameter::Category::outputMeter;
    
    juce::ignoreUnused(inMeter);
    juce::ignoreUnused(outParam);
    juce::ignoreUnused(outMeter);

    params.add
        //======================================================================
        (std::make_unique<juce::AudioProcessorParameterGroup>("filtersID", "0", "seperator0",
            //==================================================================
            std::make_unique<juce::AudioParameterFloat>("hpFrequencyID", "HP", freqRange, 632.455f, frequency, genParam),
            std::make_unique<juce::AudioParameterFloat>("lsFrequencyID", "LS", freqRange, 20.00f, frequency, genParam),
            std::make_unique<juce::AudioParameterFloat>("lsGainID", "dB", gainRange, 0.0f, decibels, genParam),
            std::make_unique<juce::AudioParameterFloat>("hsFrequencyID", "HS", freqRange, 20000.00f, frequency, genParam),
            std::make_unique<juce::AudioParameterFloat>("hsGainID", "dB", gainRange, 0.0f, decibels, genParam),
            std::make_unique<juce::AudioParameterFloat>("lpFrequencyID", "LP", freqRange, 632.455f, frequency, genParam)
            //==================================================================
            ));

    params.add
        //======================================================================
        (std::make_unique<juce::AudioProcessorParameterGroup>("masterID", "1", "seperator1",
            //==================================================================
            std::make_unique<juce::AudioParameterFloat>("outputID", "Output", outputRange, 00.00f, decibels, genParam),
            std::make_unique<juce::AudioParameterFloat>("mixID", "Mix", mixRange, 100.00f, percentage, genParam),
            std::make_unique<juce::AudioParameterBool>("bypassID", "Bypass", false)/*,
            std::make_unique<juce::AudioParameterChoice>("precisionID", "Precision", pString, 1)*/
            //==================================================================
            ));
}

//==============================================================================
