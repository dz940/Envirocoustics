#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class RotaryLookAndFeel : public LookAndFeel_V4
{
public:
    RotaryLookAndFeel()
    {
        m_iKnobImage = ImageFileFormat::loadFrom(BinaryData::knob4_png, BinaryData::knob4_pngSize);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int nWidth, int nHeight,
        float fSliderPosProportional, float fRotaryStartAngle, float fRotaryEndAngle,
        Slider& cSlider) override
    {
        (void)cSlider;
        if (m_iKnobImage.isValid())
        {
            // Determine available size for the knob
            float fAvailableSize = std::min(nWidth, nHeight) * 0.9f; // Scale knob to 90% of the slider area
            Image iResizedKnob = m_iKnobImage.rescaled((int)fAvailableSize, (int)fAvailableSize, Graphics::highResamplingQuality);

            // Calculate center position
            float fCenterX = x + nWidth * 0.5f;
            float fCenterY = y + nHeight * 0.5f;

            // Compute the rotation angle
            float fAngle = fRotaryStartAngle + fSliderPosProportional * (fRotaryEndAngle - fRotaryStartAngle);

            // Create transformation matrix
            AffineTransform transform =
                AffineTransform::translation(-fAvailableSize * 0.5f, -fAvailableSize * 0.5f) // Center image around (0,0)
                .rotated(fAngle) // Rotate image
                .translated(fCenterX, fCenterY); // Move to slider center

            // Draw the transformed image
            g.drawImageTransformed(iResizedKnob, transform, false);
        }
    }

private:
    Image m_iKnobImage;
};

class ConditionControls : public Component
{
public:
    ConditionControls(MainComponent& parentComponent);
    ~ConditionControls();
    void vSetParameter(const int nParameter, const double dValue);
    void resized() override;
    void paint(Graphics& g) override;

private:
    Slider m_cTemperatureKnob, m_cWindSpeedKnob, m_cHumidityKnob, m_cPressureKnob;
    TextButton m_cTemperatureLapseBtn, m_cTemperatureInversionBtn;
    TextButton m_cPrecipitationOnBtn, m_cPrecipitationOffBtn;
    TextButton m_cWindLeftBtn, m_cWindRightBtn;
    TextButton m_cCloudOnBtn, m_cCloudOffBtn;

    MainComponent& m_cMainComponent;
    RotaryLookAndFeel m_lfRotaryLookAndFeel;
};
