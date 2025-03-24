#pragma once

#include <JuceHeader.h>
class MainComponent;
using namespace juce;

class WeatherPresets : public Component
{
public:
    WeatherPresets(MainComponent& pcParentComponent);
    ~WeatherPresets();
    void resized() override;
    void paint(Graphics& g) override;

private:
    ImageButton m_pcSnowBtn, m_pcRainBtn, m_pcWindBtn, m_pcSunBtn;
    Image m_iSunOffImage, m_iWindOffImage, m_iRainOffImage, m_iSnowOffImage;
    Image m_iSunOnImage, m_iWindOnImage, m_iRainOnImage, m_iSnowOnImage;
    Image m_iSunDownImage, m_iWindDownImage, m_iRainDownImage, m_iSnowDownImage;

    MainComponent& m_pcMainComponent;
};
