#pragma once

#include <JuceHeader.h>
using namespace juce;

class VolumeSliderLookAndFeel : public LookAndFeel_V4
{
public:

    VolumeSliderLookAndFeel()
    {
        thumbImage = ImageFileFormat::loadFrom(BinaryData::sliderThumb_png, BinaryData::sliderThumb_pngSize);
        thumbImage = thumbImage.rescaled(30, 55, Graphics::highResamplingQuality);
    }

    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override
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

class VolumeSlider : public Slider
{
public:
    VolumeSlider();
    ~VolumeSlider() override;

    VolumeSliderLookAndFeel volumeSliderLookAndFeel;
};