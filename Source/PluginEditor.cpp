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
    // CHANNEL SELECTOR
    channelSelector.addItemList(channels, 1);
    channelSelector.onChange = [&]() { updateProfile(); };
    addAndMakeVisible(channelSelector);
    channelSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.state, "CHANNEL", channelSelector);
    
    // CAB SELECTOR
    cabSelector.addItemList(cabs, 1);
    cabSelector.onChange = [&]() { updateCab(); };
    addAndMakeVisible(cabSelector);
    cabSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.state, "CAB", cabSelector);

    addAndMakeVisible(cabToggle);
    cabAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.state, "CABENABLED", cabToggle);
    
    // BOOST
    addAndMakeVisible(boostToggle);
    boostToggle.onStateChange = [&]() { updateProfile(); };
    boostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.state, "BOOSTENABLED", boostToggle);
    
    // INPUT
    inputSlider.setName("Input");
    inputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    inputSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 50, 25);
    addAndMakeVisible(inputSlider);
    inputSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.state, "INPUT", inputSlider);
    
    // GAIN
    gainSlider.setName("Gain");
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxRight, true, 50, 25);
    addAndMakeVisible(gainSlider);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.state, "GAIN", gainSlider);
    
    // VOLUME
    volumeSlider.setName("Volume");
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
    
    g.drawText("Input", 20, getHeight() - 210, 100, 20, juce::Justification::centredLeft);
    g.drawText("Gain", 20, getHeight() - 140, 100, 20, juce::Justification::centredLeft);
    g.drawText("Volume", 20, getHeight() - 70, 100, 20, juce::Justification::centredLeft);
}

void NocturneDSPAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    channelSelector.setBounds(10, getHeight() - 450, getWidth() - 30, 40);
    boostToggle.setBounds(10, getHeight() - 400, 100, 30);
    
    cabSelector.setBounds(10, getHeight() - 350, getWidth() - 30, 40);
    cabToggle.setBounds(10, getHeight() - 300, 100, 30);
    
    inputSlider.setBounds(10, getHeight() - 200, getWidth() - 30, 50);
    gainSlider.setBounds(10, getHeight() - 130, getWidth() - 30, 50);
    volumeSlider.setBounds(10, getHeight() - 60, getWidth() - 30, 50);
}

void NocturneDSPAudioProcessorEditor::updateProfile()
{
    int choice = channelSelector.getSelectedId();
    
    if(choice == activeProfile)
    {
        return;
    }
    
    std::cout << "UPDATING PROFILE... CHOICE: " << choice << std::endl;
    
    activeProfile = choice;

    switch(activeProfile)
    {
        case 1:
            audioProcessor.loadProfile(BinaryData::model_clean_1302_210923_json);
//            audioProcessor.loadProfile("/Users/sppericat/Workspace/juce/NocturneDSP/Resources/models/model_clean_1302_210923.json");
            break;
        case 2:
            break;
        case 3:
            audioProcessor.loadProfile(BinaryData::model_rhythm_1343_210921_json);
//            audioProcessor.loadProfile("/Users/sppericat/Workspace/juce/NocturneDSP/Resources/models/model_rhythm_1343_210921.json");
            break;
        case 4:
            break;
    }
}

void NocturneDSPAudioProcessorEditor::updateCab()
{
    int choice = cabSelector.getSelectedId();
    
    std::cout << "CAB CHANGE: " << choice << std::endl;
    
    switch (choice) {
        case 1:
            audioProcessor.loadImpulseResponse(BinaryData::default_wav, BinaryData::default_wavSize);
            break;
        case 2:
            audioProcessor.loadImpulseResponse(BinaryData::chunk2_wav, BinaryData::chunk2_wavSize);
            break;
            
        default:
            break;
    }
}
