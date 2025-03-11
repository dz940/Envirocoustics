#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class VolumeSliderLookAndFeel: public LookAndFeel_V4
{
public:

    VolumeSliderLookAndFeel()
    {
        thumbImage = ImageFileFormat::loadFrom(BinaryData::sliderThumb_png, BinaryData::sliderThumb_pngSize);
        thumbImage = thumbImage.rescaled(30, 55, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    { return thumbImage.getHeight()/2; }

    void drawLinearSlider(Graphics & g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style, Slider & slider) override
    {
        (void)style;
        (void)minSliderPos;
        (void)maxSliderPos;

        auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);

        Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
            slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

        Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
            slider.isHorizontal() ? startPoint.y : (float)y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setColour(slider.findColour(Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
        auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

        minPoint = startPoint;
        maxPoint = { kx, ky };

        auto thumbWidth = thumbImage.getWidth();
        auto thumbHeight = thumbImage.getHeight();

        valueTrack.startNewSubPath(minPoint);
        valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
        g.setColour(slider.findColour(Slider::trackColourId));
        g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        // Draw tick marks
        g.setColour(Colours::white); // Set tick marks to white
        int numTicks = 8;

        for (int i = 0; i < numTicks; ++i)
        {
            float t = static_cast<float>(i) / (numTicks - 1); // Normalized position (0 to 1)
            Point<float> tickPoint = startPoint + (endPoint - startPoint) * t; // Interpolated position
            g.drawLine(tickPoint.x - 35, tickPoint.y, tickPoint.x -25, tickPoint.y, 2.0f); // Horizontal tick marks        
        }

        if (!isTwoVal)
        {
            g.drawImage(thumbImage, (width / 2) - (thumbWidth / 2), (int)sliderPos - (thumbHeight / 2),
                thumbWidth, thumbHeight, 0, 0, thumbWidth, thumbHeight);
        }

        if (slider.isBar())
        {
            drawLinearSliderOutline(g, x, y, width, height, style, slider);
        }  
    }

private:
    Image thumbImage;
};

class LevelMeter : public Component, public Timer
{
public:
    LevelMeter() : level(0.0f)
    { startTimerHz(20);  }

    void setLevel(float newLevel)
    {
        // Use a smoothing algorithm for a better visual effect
        level = 0.9f * level + 0.1f * jlimit(0.0f, 1.0f, newLevel);
        repaint();
    }

    float getLevel()
    { return level; }

    void timerCallback() override
    { repaint(); }

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::black);

        float meterWidth = (float)getWidth();
        float meterHeight = (float)getHeight();
        float levelHeight = (float)(meterHeight * (1.0f - level));

        ColourGradient gradient(
            Colours::yellow, 0.0f, (float)meterHeight,  // Yellow at the bottom
            Colours::red, 0.0f, 0.0f,            // Red at the top
            false);

        g.setGradientFill(gradient);
        g.fillRect(0.0f, levelHeight, meterWidth, meterHeight - levelHeight);
    }

private:
    float level;
};


class VolumeControl : public Component
{
public:
    VolumeControl(MainComponent& parentComponent);
    ~VolumeControl();
    double getGain();
    void resized() override;
    void setMeterLevel(float newLevel);
    void paint(Graphics& g) override;

private:
    Slider temperatureKnob, windSpeedKnob, humidityKnob;
    TextButton temperatureLapseBtn, temperatureInversionBtn;
    TextButton precipitationOnBtn, precipitationOffBtn;
    TextButton windLeftBtn, windRightBtn;

    MainComponent& mainComponent;
    Slider volumeFader;
    LevelMeter levelMeter;
    VolumeSliderLookAndFeel volumeSliderLookAndFeel;
};
