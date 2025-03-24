#pragma once
#include <JuceHeader.h>

class Spectrogram;
class MainComponent;
using namespace juce;

class SpectrogramComponent : public Component
{
public:
    SpectrogramComponent(MainComponent& pcParentComponent);
    ~SpectrogramComponent();
    void vPushBuffer(const AudioBuffer<float>& buffer);
    void resized() override;
    void paint(Graphics& g) override;

private:
    Spectrogram *m_pcSpectrogram;
    MainComponent& m_pcMainComponent;
};
