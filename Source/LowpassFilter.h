#pragma once

#include <vector>
#include <JuceHeader.h>

class LowpassFilter {
public:
    void setCutoffFrequency(float cutoffFrequency);
    void setSampleRate(float sampleRate);
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    
private:
    float cutoffFrequency;
    float sampleRate;
    std::vector<float> dnBuffer;
};

