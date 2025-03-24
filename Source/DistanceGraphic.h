#pragma once

#include <JuceHeader.h>
class MainComponent;

using namespace juce;

class StickmanSliderLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderLookAndFeel()
    {
        m_iStickManImage = ImageFileFormat::loadFrom(BinaryData::stickman_png, BinaryData::stickman_pngSize);
        m_iStickManImage = m_iStickManImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return m_iStickManImage.getHeight() / 2;
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

        bool bIsHorizontal = cSlider.isHorizontal();

        Point<float> startPoint = bIsHorizontal
            ? Point<float>((float)x, (float)y + (float)nHeight * 0.5f)
            : Point<float>((float)x + (float)nWidth * 0.5f, (float)(nHeight + y));

        Point<float> endPoint = bIsHorizontal
            ? Point<float>((float)(nWidth + x), startPoint.y)
            : Point<float>(startPoint.x, (float)y);

        // Draw Stickman Image at the correct position
        if (bIsHorizontal)
        {
            int nThumbWidth = m_iStickManImage.getWidth();
            int nThumbHeight = m_iStickManImage.getHeight();

            int nStickmanX = (int)fSliderPos - (nThumbWidth / 2);
            int nStickmanY = (int)y + (nHeight / 2) - (nThumbHeight / 2);

            g.drawImage(m_iStickManImage, nStickmanX, nStickmanY, nThumbWidth, nThumbHeight, 0, 0, nThumbWidth, nThumbHeight);
        }
    }

private:
    Image m_iStickManImage;
};


class StickmanSliderRainyLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderRainyLookAndFeel()
    {
        m_iStickManm_iRainyImage = ImageFileFormat::loadFrom(BinaryData::stickmanRainy_png, BinaryData::stickmanRainy_pngSize);
        m_iStickManm_iRainyImage = m_iStickManm_iRainyImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return m_iStickManm_iRainyImage.getHeight() / 2;
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

        bool bIsHorizontal = cSlider.isHorizontal();

        Point<float> startPoint = bIsHorizontal
            ? Point<float>((float)x, (float)y + (float)nHeight * 0.5f)
            : Point<float>((float)x + (float)nWidth * 0.5f, (float)(nHeight + y));

        Point<float> endPoint = bIsHorizontal
            ? Point<float>((float)(nWidth + x), startPoint.y)
            : Point<float>(startPoint.x, (float)y);

        // Draw Stickman Image at the correct position
        if (bIsHorizontal)
        {
            int nThumbWidth = m_iStickManm_iRainyImage.getWidth();
            int nThumbHeight = m_iStickManm_iRainyImage.getHeight();

            int nStickmanX = (int)fSliderPos - (nThumbWidth / 2);
            int nStickmanY = (int)y + (nHeight / 2) - (nThumbHeight / 2);

            g.drawImage(m_iStickManm_iRainyImage, nStickmanX, nStickmanY, nThumbWidth, nThumbHeight, 0, 0, nThumbWidth, nThumbHeight);
        }
    }

private:
    Image m_iStickManm_iRainyImage;
};

class StickmanSliderSnowyLookAndFeel : public LookAndFeel_V4
{
public:
    StickmanSliderSnowyLookAndFeel()
    {
        m_iStickManm_iSnowyImage = ImageFileFormat::loadFrom(BinaryData::stickmanSnowy_png, BinaryData::stickmanSnowy_pngSize);
        m_iStickManm_iSnowyImage = m_iStickManm_iSnowyImage.rescaled(54, 57, Graphics::highResamplingQuality);
    }

    int getSliderThumbRadius(Slider&) override
    {
        return m_iStickManm_iSnowyImage.getHeight() / 2;
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

        bool bIsHorizontal = cSlider.isHorizontal();

        Point<float> startPoint = bIsHorizontal
            ? Point<float>((float)x, (float)y + (float)nHeight * 0.5f)
            : Point<float>((float)x + (float)nWidth * 0.5f, (float)(nHeight + y));

        Point<float> endPoint = bIsHorizontal
            ? Point<float>((float)(nWidth + x), startPoint.y)
            : Point<float>(startPoint.x, (float)y);  

        // Draw Stickman Image at the correct position 
        if (bIsHorizontal)
        {
            int nThumbWidth = m_iStickManm_iSnowyImage.getWidth();
            int nThumbHeight = m_iStickManm_iSnowyImage.getHeight();

            int nStickmanX = (int)fSliderPos - (nThumbWidth / 2);
            int nStickmanY = (int)y + (nHeight / 2) - (nThumbHeight / 2);

            g.drawImage(m_iStickManm_iSnowyImage, nStickmanX, nStickmanY, nThumbWidth, nThumbHeight, 0, 0, nThumbWidth, nThumbHeight);
        }
    }

private:
    Image m_iStickManm_iSnowyImage;
};

class DistanceGraphic : public Component
{
public:
    DistanceGraphic(MainComponent& pcParentComponent);
    ~DistanceGraphic();
    void resized() override;
    void paint(Graphics& g) override;

private:
    Slider m_cDistanceSlider;
    TextEditor m_cDistanceText;
    Image m_iSunnyImage, m_iWindyImage, m_iRainyImage, m_iSnowyImage;

    Image m_iWindLeft1, m_iWindLeft2, m_iWindLeft3, m_iWindLeft4, m_iWindLeft5;
    Image m_iWindRight1, m_iWindRight2, m_iWindRight3, m_iWindRight4, m_iWindRight5;
    Image m_iStageNormal, m_iStageRainy, m_iStageSnowy;
    Image m_iRainOverlay, m_iSnowOverlay, m_iSunOverlay;
    Image m_iCloudsPartial, m_iCloudsLight, m_iCloudsDark;

    MainComponent& m_pcMainComponent;
    StickmanSliderLookAndFeel m_lfStickmanLookAndFeel;
    StickmanSliderRainyLookAndFeel m_lfStickmanRainyLookAndFeel;
    StickmanSliderSnowyLookAndFeel m_lfStickmanSnowyLookAndFeel;
};
