#pragma once

#include <JuceHeader.h>
class MainComponent;
using namespace juce;

class WaveformDisplay : public Component
{
public:
    WaveformDisplay(MainComponent& pcParentComponent);
    ~WaveformDisplay();
    void vPushBuffer(const AudioBuffer<float>& buffer);
    void resized() override;
    void paint(Graphics& g) override;

private:
    AudioVisualiserComponent m_cVisualiser{ 1 };
    MainComponent& m_pcMainComponent;
};
