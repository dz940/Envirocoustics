#pragma once

#include <JuceHeader.h>
class MainComponent;
using namespace juce;

class WaveformDisplay : public Component
{
public:
    WaveformDisplay(MainComponent& parentComponent);
    ~WaveformDisplay();
    void pushBuffer(const AudioBuffer<float>& buffer);
    void resized() override;
    void paint(Graphics& g) override;

private:
    AudioVisualiserComponent visualiser{ 1 };
    MainComponent& mainComponent;
};
