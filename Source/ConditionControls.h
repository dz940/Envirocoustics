#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class RotaryLookAndFeel : public LookAndFeel_V4
{
public:
    RotaryLookAndFeel()
    {
        knobImage = ImageFileFormat::loadFrom(BinaryData::knob5_png, BinaryData::knob5_pngSize);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
        Slider& slider) override
    {
        (void)slider;
        if (knobImage.isValid())
        {
            // Determine available size for the knob
            float availableSize = std::min(width, height) * 0.9f; // Scale knob to 90% of the slider area
            Image resizedKnob = knobImage.rescaled((int)availableSize, (int)availableSize, Graphics::highResamplingQuality);

            // Calculate center position
            float centerX = x + width * 0.5f;
            float centerY = y + height * 0.5f;

            // Compute the rotation angle
            float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

            // Create transformation matrix
            AffineTransform transform =
                AffineTransform::translation(-availableSize * 0.5f, -availableSize * 0.5f) // Center image around (0,0)
                .rotated(angle) // Rotate image
                .translated(centerX, centerY); // Move to slider center

            // Draw the transformed image
            g.drawImageTransformed(resizedKnob, transform, false);
        }
    }

private:
    Image knobImage;
};

class ConditionControls : public Component
{
public:
    ConditionControls(MainComponent& parentComponent);
    ~ConditionControls();
    void vSetParameter(int nParameter, int nValue);
    void resized() override;
    void paint(Graphics& g) override;

private:
    Slider temperatureKnob, windSpeedKnob, humidityKnob, pressureKnob;
    TextButton temperatureLapseBtn, temperatureInversionBtn;
    TextButton precipitationOnBtn, precipitationOffBtn;
    TextButton windLeftBtn, windRightBtn;
    TextButton cloudOnBtn, cloudOffBtn;

    MainComponent& mainComponent;
    RotaryLookAndFeel rotaryLookAndFeel;
};
