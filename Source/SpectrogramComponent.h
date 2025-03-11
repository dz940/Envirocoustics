#pragma once
#include <JuceHeader.h>

class Spectrogram;
class MainComponent;
using namespace juce;

class SpectrogramComponent : public Component
{
public:
    SpectrogramComponent(MainComponent& parentComponent);
    ~SpectrogramComponent();
    void pushBuffer(const AudioBuffer<float>& buffer);
    void resized() override;
    void paint(Graphics& g) override;

private:
    Spectrogram *spectrogram;
    MainComponent& mainComponent;
    bool gradientDrawn = false;
};
