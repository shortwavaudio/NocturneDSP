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
    
    void updateProfile();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NocturneDSPAudioProcessor& audioProcessor;
    
    juce::Slider gainSlider, volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment, volumeSliderAttachment;
    
    juce::ComboBox cabSelector, channelSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cabSelectorAttachment, channelSelectorAttachment;
    
    juce::ToggleButton boostToggle { "BOOST" };
    juce::ToggleButton cabToggle { "CAB ENABLED" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabAttachment, boostAttachment;
    
    const juce::StringArray cabs = {
        "default"
    };
    
    const juce::StringArray channels = {
        "clean",
        "crunch",
        "rhythm",
        "lead"
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NocturneDSPAudioProcessorEditor)
};
