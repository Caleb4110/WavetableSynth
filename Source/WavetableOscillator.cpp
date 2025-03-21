#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator(std::vector<float> wavetable, double sampleRate)
    : wavetable{ std::move(wavetable) },
      sampleRate{ sampleRate }
{}

void WavetableOscillator::setFrequency(float frequency)
{
    indexIncrement = frequency * static_cast<float>(wavetable.size()) / static_cast<float>(sampleRate);
}

float WavetableOscillator::getSample()
{
    const auto sample = interpolateLinearly();
    index += indexIncrement;
    index = std::fmod(index, static_cast<float>(wavetable.size()));
    return sample;
}

float WavetableOscillator::interpolateLinearly()
{
    const auto truncatedIndex = static_cast<int>(index);
    const auto nextIndex = (truncatedIndex + 1) % static_cast<int>(wavetable.size());
    
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    const auto truncatedIndexWeight = 1.f - nextIndexWeight;
    
    return truncatedIndexWeight * wavetable[truncatedIndex] + nextIndexWeight * wavetable[nextIndex];
}

void WavetableOscillator::stop()
{
    index = 0.f;
    indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying()
{
    return indexIncrement != 0.f;
}
