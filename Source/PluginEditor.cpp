/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAudioProcessorEditor::WavetableSynthAudioProcessorEditor (WavetableSynthAudioProcessor& p, juce::AudioProcessorValueTreeState& valueTree)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    
    // Cutoff frequency dial and label
    addAndMakeVisible(cutoffDial);
    cutoffDial.setSliderStyle(juce::Slider::Rotary);
    cutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTree, "cutoff", cutoffDial));
    
    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("CUTOFF", juce::dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffDial, false);
    

    // TODO: add waveform dial to the valueTreeState
    // Waveform dial for oscillator 1
    waveformDial.setRange(0, 2, 1);
    waveformDial.setSliderStyle(juce::Slider::Rotary);
    waveformDial.addListener(this);
    waveformDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                                 true, 0, 0);
    // Change the oscillators waveform when the dial changes
    waveformDial.onValueChange = [this] { audioProcessor.updateSynthWaveform(static_cast<int>(waveformDial.getValue())); };
    
    // Waveform label for oscillator 1
    waveformLabel.setText("WAVEFORM", juce::dontSendNotification);
    waveformLabel.attachToComponent(&waveformDial, false);
    
    // Make both visible
    addAndMakeVisible(waveformDial);
    addAndMakeVisible(waveformLabel);
}

WavetableSynthAudioProcessorEditor::~WavetableSynthAudioProcessorEditor()
{
}

//==============================================================================
void WavetableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void WavetableSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    waveformDial.setBounds(50, 50, 120, 120);
    cutoffDial.setBounds(15, 35, 100, 300);
}

void WavetableSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

}
