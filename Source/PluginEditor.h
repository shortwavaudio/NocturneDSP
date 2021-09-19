/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NocturneDSPAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NocturneDSPAudioProcessorEditor (NocturneDSPAudioProcessor&);
    ~NocturneDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NocturneDSPAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider, volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment, volumeSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NocturneDSPAudioProcessorEditor)
};
