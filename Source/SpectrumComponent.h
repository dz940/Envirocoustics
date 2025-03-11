#pragma once
#include <JuceHeader.h>

class FrequencySpectrum;
class MainComponent;

using namespace juce;

class SpectrumComponent : public juce::Component
{
public:
    // Constructor
    SpectrumComponent(MainComponent& parentComponent);
    ~SpectrumComponent();

    void pushBuffer(const juce::AudioBuffer<float>& buffer);

    // Component override to handle resizing
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    FrequencySpectrum* freqSpectrum;
    MainComponent& mainComponent;
    bool gradientDrawn = false;
};
