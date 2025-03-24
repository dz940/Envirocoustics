#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class VolumeSliderLookAndFeel: public LookAndFeel_V4
{
public:

    VolumeSliderLookAndFeel()
    {
        m_iThumbImage = ImageFileFormat::loadFrom(BinaryData::sliderThumb_png, BinaryData::sliderThumb_pngSize);
        m_iThumbImage = m_iThumbImage.rescaled(30, 55, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    { return m_iThumbImage.getHeight()/2; }

    void drawLinearSlider(Graphics & g, int x, int y, int nWidth, int nHeight,
        float fSliderPos,
        float fMinSliderPos,
        float fMaxSliderPos,
        const Slider::SliderStyle cStyle, Slider & cSlider) override
    {
        (void)cStyle;
        (void)fMinSliderPos;
        (void)fMaxSliderPos;

        bool bIsTwoVal = (cStyle == Slider::SliderStyle::TwoValueVertical || cStyle == Slider::SliderStyle::TwoValueHorizontal);
        bool bIsThreeVal = (cStyle == Slider::SliderStyle::ThreeValueVertical || cStyle == Slider::SliderStyle::ThreeValueHorizontal);

        float fTrackWidth = jmin(6.0f, cSlider.isHorizontal() ? (float)nHeight * 0.25f : (float)nWidth * 0.25f);

        Point<float> fpStartPoint(cSlider.isHorizontal() ? (float)x : (float)x + (float)nWidth * 0.5f,
            cSlider.isHorizontal() ? (float)y + (float)nHeight * 0.5f : (float)(nHeight + y));

        Point<float> fpEndPoint(cSlider.isHorizontal() ? (float)(nWidth + x) : fpStartPoint.x,
            cSlider.isHorizontal() ? fpStartPoint.y : (float)y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath(fpStartPoint);
        backgroundTrack.lineTo(fpEndPoint);
        g.setColour(cSlider.findColour(Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { fTrackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> fpMinPoint, fpMaxPoint, fpThumbPoint;

        auto kx = cSlider.isHorizontal() ? fSliderPos : ((float)x + (float)nWidth * 0.5f);
        auto ky = cSlider.isHorizontal() ? ((float)y + (float)nHeight * 0.5f) : fSliderPos;

        fpMinPoint = fpStartPoint;
        fpMaxPoint = { kx, ky };

        int nThumbWidth = m_iThumbImage.getWidth();
        int nThumbHeight = m_iThumbImage.getHeight();

        valueTrack.startNewSubPath(fpMinPoint);
        valueTrack.lineTo(bIsThreeVal ? fpThumbPoint : fpMaxPoint);
        g.setColour(cSlider.findColour(Slider::trackColourId));
        g.strokePath(valueTrack, { fTrackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        // Draw tick marks
        g.setColour(Colours::white); // Set tick marks to white
        int nNumTicks = 8;

        for (int i = 0; i < nNumTicks; ++i)
        {
            float t = static_cast<float>(i) / (nNumTicks - 1); // Normalized position (0 to 1)
            Point<float> fpTickPoint = fpStartPoint + (fpEndPoint - fpStartPoint) * t; // Interpolated position
            g.drawLine(fpTickPoint.x - 35, fpTickPoint.y, fpTickPoint.x -25, fpTickPoint.y, 2.0f); // Horizontal tick marks        
        }

        if (!bIsTwoVal)
        {
            g.drawImage(m_iThumbImage, (nWidth / 2) - (nThumbWidth / 2), (int)fSliderPos - (nThumbHeight / 2),
                nThumbWidth, nThumbHeight, 0, 0, nThumbWidth, nThumbHeight);
        }

        if (cSlider.isBar())
        {
            drawLinearSliderOutline(g, x, y, nWidth, nHeight, cStyle, cSlider);
        }  
    }

private:
    Image m_iThumbImage;
};

class LevelMeter : public Component
{
public:
    LevelMeter() : m_fLevel(0.0f)
    {}

    void vSetLevel(const float fNewLevel)
    {
        // Use a smoothing algorithm for a better visual effect
        m_fLevel = 0.9f * m_fLevel + 0.1f * jlimit(0.0f, 1.0f, fNewLevel);
        repaint();
    }

    float fGetLevel()
    { return m_fLevel; }

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::black);

        float fMeterWidth = (float)getWidth();
        float fMeterHeight = (float)getHeight();
        float fLevelHeight = (float)(fMeterHeight * (1.0f - m_fLevel));

        ColourGradient gradient(
            Colours::yellow, 0.0f, (float)fMeterHeight,  // Yellow at the bottom
            Colours::red, 0.0f, 0.0f,            // Red at the top
            false);

        g.setGradientFill(gradient);
        g.fillRect(0.0f, fLevelHeight, fMeterWidth, fMeterHeight - fLevelHeight);
    }

private:
    float m_fLevel;
};


class VolumeControl : public Component
{
public:
    VolumeControl(MainComponent& parentComponent);
    ~VolumeControl();
    double dGetGain();
    void resized() override;
    void vSetMeterLevel(float fNewLevel);
    void paint(Graphics& g) override;

private:

    MainComponent& m_pcMainComponent;
    Slider m_cVolumeFader;
    LevelMeter m_cLevelMeter;
    VolumeSliderLookAndFeel m_lfVolumeSliderLookAndFeel;
};
