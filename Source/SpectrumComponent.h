#pragma once
#include <JuceHeader.h>

class FrequencySpectrum;
class MainComponent;

using namespace juce;

class SpectrumComponent : public Component
{
public:
    // Constructor
    SpectrumComponent(MainComponent& parentComponent);
    ~SpectrumComponent();

    void vPushBuffer(const AudioBuffer<float>& buffer);
    // Component override to handle resizing
    void resized() override;
    void paint(Graphics& g) override;

private:
    FrequencySpectrum* m_pcFreqSpectrum;
    MainComponent& m_pcMainComponent;
};
