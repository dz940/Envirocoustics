#pragma once

#include <JuceHeader.h>
using namespace juce;

class VolumeSliderLookAndFeel : public LookAndFeel_V4
{
public:

    VolumeSliderLookAndFeel()
    {
        m_iThumbImage = ImageFileFormat::loadFrom(BinaryData::sliderThumb_png, BinaryData::sliderThumb_pngSize);
        m_iThumbImage = m_iThumbImage.rescaled(30, 55, Graphics::highResamplingQuality);
    }

    void drawLinearSlider(Graphics& g, int x, int y, int nWidth, int nHeight,
        float fSliderPos,
        float fMinSliderPos,
        float fMaxSliderPos,
        const Slider::SliderStyle cStyle, Slider& cSlider) override
    {
        (void)cStyle;
        (void)fMinSliderPos;
        (void)fMaxSliderPos;
        
        bool bIsTwoVal = (cStyle == Slider::SliderStyle::TwoValueVertical || cStyle == Slider::SliderStyle::TwoValueHorizontal);
        bool bIsThreeVal = (cStyle == Slider::SliderStyle::ThreeValueVertical || cStyle == Slider::SliderStyle::ThreeValueHorizontal);

        float fTrackWidth = jmin(6.0f, cSlider.isHorizontal() ? (float)nHeight * 0.25f : (float)nWidth * 0.25f);

        Point<float> pfStartPoint(cSlider.isHorizontal() ? (float)x : (float)x + (float)nWidth * 0.5f,
            cSlider.isHorizontal() ? (float)y + (float)nHeight * 0.5f : (float)(nHeight + y));

        Point<float> pfEndPoint(cSlider.isHorizontal() ? (float)(nWidth + x) : pfStartPoint.x,
            cSlider.isHorizontal() ? pfStartPoint.y : (float)y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath(pfStartPoint);
        backgroundTrack.lineTo(pfEndPoint);
        g.setColour(cSlider.findColour(Slider::backgroundColourId));
        g.strokePath(backgroundTrack, { fTrackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> pfMinPoint, pfMaxPoint, pfThumbPoint;
        
        float kx = cSlider.isHorizontal() ? fSliderPos : ((float)x + (float)nWidth * 0.5f);
        float ky = cSlider.isHorizontal() ? ((float)y + (float)nHeight * 0.5f) : fSliderPos;

        pfMinPoint = pfStartPoint;
        pfMaxPoint = { kx, ky };

        int nThumbWidth = m_iThumbImage.getWidth();
        int nThumbHeight = m_iThumbImage.getHeight();

        valueTrack.startNewSubPath(pfMinPoint);
        valueTrack.lineTo(bIsThreeVal ? pfThumbPoint : pfMaxPoint);
        g.setColour(cSlider.findColour(Slider::trackColourId));
        g.strokePath(valueTrack, { fTrackWidth, PathStrokeType::curved, PathStrokeType::rounded });

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

class VolumeSlider : public Slider
{
public:
    VolumeSlider();
    ~VolumeSlider() override;

private:
    VolumeSliderLookAndFeel m_lfVolumeSliderLookAndFeel;
};