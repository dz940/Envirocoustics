#pragma once

#include <JuceHeader.h>

class MainComponent;

class FrequencyAnalyzerComponent : public juce::Component,
    public juce::Timer
{
public:
    FrequencyAnalyzerComponent(MainComponent& parentComponent);
    ~FrequencyAnalyzerComponent() override;

    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock);
    void releaseResources();

    void processAudioBlock(const juce::AudioBuffer<float>& buffer);
    void performFFT();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;

private:
    int fftSize;
    double sampleRate;

    std::unique_ptr<juce::dsp::FFT> fft;

    std::vector<float> fftData;
    std::vector<float> frequencyMagnitudes;

    std::vector<float> fftBuffer; // A temporary buffer to hold FFT data
    MainComponent& mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyAnalyzerComponent)
};
