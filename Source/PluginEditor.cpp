/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NocturneDSPAudioProcessorEditor::NocturneDSPAudioProcessorEditor (NocturneDSPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // VOLUME
    volumeSlider.setName("Input");
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 50, 25);
    addAndMakeVisible(volumeSlider);
    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.state, "VOLUME", volumeSlider);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 500);
}

NocturneDSPAudioProcessorEditor::~NocturneDSPAudioProcessorEditor()
{
}

//==============================================================================
void NocturneDSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Nocturne DSP Kitchen Sink", getLocalBounds().reduced(10), juce::Justification::centredTop, 1);
    
    g.drawText("Volume", 20, getHeight() - 70, 100, 20, juce::Justification::centredLeft);
}

void NocturneDSPAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    volumeSlider.setBounds(10, getHeight() - 60, getWidth() - 30, 50);
}
