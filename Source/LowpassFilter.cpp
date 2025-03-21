#include "LowpassFilter.h"

void LowpassFilter::setCutoffFrequency(float cutoffFrequency)
{
    this->cutoffFrequency = cutoffFrequency;
}
void LowpassFilter::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}

void LowpassFilter::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const auto PI = std::atanf(1.f) * 4;
    const auto numChannels = buffer.getNumChannels();
    
    dnBuffer.resize(numChannels, 0.f);
    
    // For highpass filtering, subtract, lowpass is addition
    const auto tan = std::tan(PI * cutoffFrequency / sampleRate);
    const auto a1 = (tan -1.f) / (tan + 1.f);
    
    for (auto channel = 0; channel < numChannels; ++channel)
    {
        auto channelSamples = buffer.getWritePointer(channel);
        
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = channelSamples[i];
            
            const auto filteredSample = a1 * inputSample + dnBuffer[channel];
            dnBuffer[channel] = inputSample - a1 * filteredSample;
            
            // add the filtered sample to the input sample and scale down
            // by half so we don't double the amplitude
            const auto filteredOutput = 0.5f * (inputSample + filteredSample);
            
            channelSamples[i] = filteredOutput;
        }
    }
}
