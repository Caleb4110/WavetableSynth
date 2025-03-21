#pragma once
#include <JuceHeader.h>

class WavetableOscillator
{
public:
    WavetableOscillator(std::vector<float> wavetable, double sampleRate);
    void setFrequency(float frequency);
    float getSample();
    void stop();
    bool isPlaying();

private:
    float interpolateLinearly();
    
    std::vector<float> wavetable;
    double sampleRate;
    float index = 0.f;
    float indexIncrement = 0.f;
};
