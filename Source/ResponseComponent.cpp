#include "ResponseComponent.h"
#include "MainComponent.h"
#include "ResponseCurve.h"

/*======================================================================================*/
ResponseComponent::ResponseComponent(MainComponent& parentComponent)
    : m_pcMainComponent(parentComponent)
    /*======================================================================================*/
{
    m_pcResponseCurve = new ResponseCurve(m_pcMainComponent);
    addAndMakeVisible(m_pcResponseCurve);
    m_pcResponseCurve->setOpaque(true);
}

/*======================================================================================*/
ResponseComponent::~ResponseComponent()
/*======================================================================================*/
{
    delete m_pcResponseCurve;
}

/*======================================================================================*/
void ResponseComponent::resized()
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    m_pcResponseCurve->setBounds(rcRect.getX() + 35, rcRect.getY() + 30, rcRect.getWidth() - 50, rcRect.getHeight() - 45);

}
/*======================================================================================*/
void ResponseComponent::vSetWindCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs)
/*======================================================================================*/
{
    m_pcResponseCurve->vSetWindCoefficients(newCoeffs);
}

/*======================================================================================*/
void ResponseComponent::vSetTempGradientCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs)
/*======================================================================================*/
{
    m_pcResponseCurve->vSetTempGradientCoefficients(newCoeffs);
}

/*======================================================================================*/
void ResponseComponent::paint(Graphics& g)
/*======================================================================================*/
{
    g.fillAll(Colours::darkseagreen);
    g.setColour(Colours::black);
    g.setFont(20.0f);

    // Title
    g.drawText("System Response Curve", 0, 0, getWidth(), 30, Justification::centred, true);

    // Define drawing bounds
    float fWidth = (float)getWidth();
    float fHeight = (float)getHeight();

    float fTopOffset = 30.0f;        // 30px for title
    float fBottomOffset = 15.0f;     // 15px space at bottom for labels
    float fLeftMargin = 35.0f;       // Reduced left margin, still enough for dB labels
    float fRightMargin = 15.0f;      // Ensure space on the right
    float fGraphWidth = fWidth - fLeftMargin - fRightMargin;
    float fGraphHeight = fHeight - fTopOffset - fBottomOffset;

    Path responseCurve;
    g.setFont(10.0f); // Smaller font for labels

    // Draw Magnitude Labels (+10 dB at top, -50 dB at bottom)
    for (int ndB = 10; ndB >= -50; ndB -= 10)
    {
        float y = fTopOffset + jmap((float)ndB, 10.0f, -50.0f, 0.0f, fGraphHeight);

        g.setColour(Colours::black);
        g.drawText(String(ndB) + " dB", 0, (int)(y - 5), 30, 10, Justification::right, false);
    }

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> dMajorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };

    for (double dFreq : dMajorFreqs)
    {
        float x = fLeftMargin + jmap((float)std::log10(dFreq), 1.3f, 4.3f, 0.0f, fGraphWidth);

        g.setColour(Colours::black);
        if (dFreq < 1000)
        {
            g.drawText(String((int)dFreq) + "Hz", (int)(x - 15), (int)(fHeight - 15), 30, 15, Justification::centred, false);
        }
        else
        {
            int nTempFreq = (int)(dFreq / 1000);
            g.drawText(String((int)nTempFreq) + "kHz", (int)(x - 15), (int)(fHeight - 15), 30, 15, Justification::centred, false);
        }
    }
}