#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    
    // Generate all tables
    sineTable = generateSineWavetable();
    sawTable = generateSawWavetable();
    triangleTable = generateTriangleWavetable();
    
    // Initialise the oscillators with the sine table
    initialiseOscillators(0);
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;
    
    for (const auto midiMessage : midiMessages)
    {
        const auto midiEvent = midiMessage.getMessage();
        const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());
        
        render(buffer, currentSample, midiEventSample);
        handleMidiEvent(midiEvent);
        
        currentSample = midiEventSample;
    }
    
    render(buffer, currentSample, buffer.getNumSamples());
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
    if (midiEvent.isNoteOn())
    {
        const auto oscillatorId = midiEvent.getNoteNumber();
        const auto frequency = midiNoteNumberToFrequency(oscillatorId);
        oscillators[oscillatorId].setFrequency(frequency);
    }
    else if (midiEvent.isNoteOff())
    {
        const auto oscillatorId = midiEvent.getNoteNumber();
        oscillators[oscillatorId].stop();
    }
    else if (midiEvent.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
    }
}

float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
    constexpr auto A4_FREQUENCY = 440.f;
    constexpr auto A4_NOTE_NUMBER = 69.f;
    constexpr auto SEMITONES_PER_OCTAVE = 12.f;
    return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_PER_OCTAVE);
}

void WavetableSynth::initialiseOscillators(int tableNumber)
{
    constexpr auto OSCILLATORS_COUNT = 128; // One oscillator for every key
    auto wavetable = tableNumber == 0 ? sineTable : tableNumber == 1 ? triangleTable : sawTable;
    
    oscillators.clear();
    for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
    {
        oscillators.emplace_back(wavetable, sampleRate);
    }
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);
    
    for (auto& oscillator: oscillators)
    {
        if (oscillator.isPlaying())
        {
            for (auto sample = startSample; sample < endSample; ++sample)
            {
                firstChannel[sample] += oscillator.getSample() * 0.25; // 0.25 is gain
            }
        }
    }
    
    for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        std::copy(firstChannel + startSample, firstChannel + endSample, buffer.getWritePointer(channel) + startSample);
    }
}

std::vector<float> WavetableSynth::generateSineWavetable()
{
    constexpr auto WAVETABLE_SIZE = 64;
    
    std::vector<float> sineWavetable(WAVETABLE_SIZE);
    
    const auto PI = std::atanf(1.f) * 4;
    
    for (auto i = 0; i < WAVETABLE_SIZE; ++i)
    {
        sineWavetable[i] = std::sinf(2 * PI * static_cast<float>(i) / static_cast<float>(WAVETABLE_SIZE));
    }
    
    return sineWavetable;
}

std::vector<float> WavetableSynth::generateSawWavetable()
{
    constexpr auto WAVETABLE_SIZE = 64;
    
    std::vector<float> sawWavetable(WAVETABLE_SIZE);
    
    for (auto i = 0; i < WAVETABLE_SIZE; ++i)
    {
        sawWavetable[i] = 2 * static_cast<float>(i) / static_cast<float>(WAVETABLE_SIZE) - 1.0f;
    }
    
    return sawWavetable;
}

std::vector<float> WavetableSynth::generateTriangleWavetable()
{
    constexpr auto WAVETABLE_SIZE = 64;
    
    std::vector<float> triangleWavetable(WAVETABLE_SIZE);
    
    for (auto i = 0; i < WAVETABLE_SIZE; ++i)
    {
        triangleWavetable[i] = 2 * abs(2 * (static_cast<float>(i) / static_cast<float>(WAVETABLE_SIZE)
                                            - floor(static_cast<float>(i) / static_cast<float>(WAVETABLE_SIZE) + 0.5f)));
    }
    
    return triangleWavetable;
}

