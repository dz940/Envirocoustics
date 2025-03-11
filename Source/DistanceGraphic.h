#pragma once

#include <JuceHeader.h>
class MainComponent;

using namespace juce;

class StickmanSliderLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderLookAndFeel()
    {
        stickManImage = ImageFileFormat::loadFrom(BinaryData::stickman_png, BinaryData::stickman_pngSize);
        stickManImage = stickManImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return stickManImage.getHeight() / 2;
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

        bool isHorizontal = slider.isHorizontal();

        Point<float> startPoint = isHorizontal
            ? Point<float>((float)x, (float)y + (float)height * 0.5f)
            : Point<float>((float)x + (float)width * 0.5f, (float)(height + y));

        Point<float> endPoint = isHorizontal
            ? Point<float>((float)(width + x), startPoint.y) 
            : Point<float>(startPoint.x, (float)y);

        // Draw Stickman Image at the correct position
        if (isHorizontal)
        {
            int thumbWidth = stickManImage.getWidth();
            int thumbHeight = stickManImage.getHeight();

            int stickmanX = (int)sliderPos - (thumbWidth / 2);
            int stickmanY = (int)y + (height / 2) - (thumbHeight / 2);

            g.drawImage(stickManImage, stickmanX, stickmanY, thumbWidth, thumbHeight, 0, 0, thumbWidth, thumbHeight);
        }
    }

private:
    Image stickManImage;
};


class StickmanSliderRainyLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderRainyLookAndFeel()
    {
        stickManRainyImage = ImageFileFormat::loadFrom(BinaryData::stickmanRainy_png, BinaryData::stickmanRainy_pngSize);
        stickManRainyImage = stickManRainyImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return stickManRainyImage.getHeight() / 2;
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

        bool isHorizontal = slider.isHorizontal();

        Point<float> startPoint = isHorizontal
            ? Point<float>((float)x, (float)y + (float)height * 0.5f)
            : Point<float>((float)x + (float)width * 0.5f, (float)(height + y));

        Point<float> endPoint = isHorizontal
            ? Point<float>((float)(width + x), startPoint.y)
            : Point<float>(startPoint.x, (float)y);

        // Draw Stickman Image at the correct position
        if (isHorizontal)
        {
            int thumbWidth = stickManRainyImage.getWidth();
            int thumbHeight = stickManRainyImage.getHeight();

            int stickmanX = (int)sliderPos - (thumbWidth / 2);
            int stickmanY = (int)y + (height / 2) - (thumbHeight / 2);

            g.drawImage(stickManRainyImage, stickmanX, stickmanY, thumbWidth, thumbHeight, 0, 0, thumbWidth, thumbHeight);
        }
    }

private:
    Image stickManRainyImage;
};

class StickmanSliderSnowyLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderSnowyLookAndFeel()
    {
        stickManSnowyImage = ImageFileFormat::loadFrom(BinaryData::stickmanSnowy_png, BinaryData::stickmanSnowy_pngSize);
        stickManSnowyImage = stickManSnowyImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return stickManSnowyImage.getHeight() / 2;
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

        bool isHorizontal = slider.isHorizontal();

        Point<float> startPoint = isHorizontal
            ? Point<float>((float)x, (float)y + (float)height * 0.5f)
            : Point<float>((float)x + (float)width * 0.5f, (float)(height + y)); 

        Point<float> endPoint = isHorizontal
            ? Point<float>((float)(width + x), startPoint.y) 
            : Point<float>(startPoint.x, (float)y);  

        // Draw Stickman Image at the correct position 
        if (isHorizontal)
        {
            int thumbWidth = stickManSnowyImage.getWidth();
            int thumbHeight = stickManSnowyImage.getHeight();

            int stickmanX = (int)sliderPos - (thumbWidth / 2); 
            int stickmanY = (int)y + (height / 2) - (thumbHeight / 2); 

            g.drawImage(stickManSnowyImage, stickmanX, stickmanY, thumbWidth, thumbHeight, 0, 0, thumbWidth, thumbHeight);
        }
    }

private:
    Image stickManSnowyImage;
};

class DistanceGraphic : public Component
{
public:
    DistanceGraphic(MainComponent& parentComponent);
    ~DistanceGraphic();
    void resized() override;
    void paint(Graphics& g) override;

private:
    Slider distanceSlider;
    TextEditor distanceText;
    Image sunnyImage, windyImage, rainyImage, snowyImage, stickFigureImage;

    Image windLeft1, windLeft2, windLeft3, windLeft4, windLeft5;
    Image windRight1, windRight2, windRight3, windRight4, windRight5;
    Image stageNormal, stageRainy, stageSnowy;
    Image rainOverlay, snowOverlay, sunOverlay;
    Image cloudsPartial, cloudsLight, cloudsDark;

    MainComponent& mainComponent;
    StickmanSliderLookAndFeel stickmanLookAndFeel;
    StickmanSliderRainyLookAndFeel stickmanRainyLookAndFeel;
    StickmanSliderSnowyLookAndFeel stickmanSnowyLookAndFeel;
};
