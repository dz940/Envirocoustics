#pragma once

#include <JuceHeader.h>
class MainComponent;
using namespace juce;

class WeatherPresets : public Component
{
public:
    WeatherPresets(MainComponent& parentComponent);
    ~WeatherPresets();
    void resized() override;
    void paint(Graphics& g) override;

private:
    ImageButton snowBtn, rainBtn, windBtn, sunBtn;
    Image sunOffImage, windOffImage, rainOffImage, snowOffImage;
    Image sunOnImage, windOnImage, rainOnImage, snowOnImage;
    Image sunDownImage, windDownImage, rainDownImage, snowDownImage;

    MainComponent& mainComponent;
};
