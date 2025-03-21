#pragma once
#include <JuceHeader.h>
#include "WavetableOscillator.h"

class WavetableSynth
{
    
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void initialiseOscillators(int tableNumber);

private:
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(int midiNoteNumber);
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    
    
    std::vector<float> generateSineWavetable();
    std::vector<float> generateSawWavetable();
    std::vector<float> generateTriangleWavetable();
    
    double sampleRate;
    
    std::vector<float> sineTable;
    std::vector<float> triangleTable;
    std::vector<float> sawTable; 
    
    std::vector<WavetableOscillator> oscillators;
};
