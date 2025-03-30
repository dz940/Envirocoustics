#include "ResponseCurve.h"
#include "MainComponent.h"
#include <math.h>

/*======================================================================================*/
ResponseCurve::ResponseCurve(MainComponent& parentComponent)
    : m_pcMainComponent(parentComponent)
/*======================================================================================*/
{

}

/*======================================================================================*/
ResponseCurve::~ResponseCurve()
/*======================================================================================*/
{

}

/*======================================================================================*/
void ResponseCurve::paint(Graphics& g)
/*======================================================================================*/
{
    g.fillAll(Colours::darkseagreen);

    // Define drawing bounds
    float fWidth = (float)getWidth();
    float fHeight = (float)getHeight();

    Path responseCurve;
    g.setFont(10.0f); // Smaller font for labels

    // Draw horizontal grid lines
    for (int ndB = 10; ndB >= -50; ndB -= 10)
    {
        float y = jmap((float)ndB, 10.0f, -50.0f, 0.0f, fHeight);

        // Draw horizontal grid line
        g.setColour(Colours::grey.withAlpha(0.3f));
        g.drawLine(0, y, fWidth, y);
    }

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> dMajorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };

    for (double dFreq : dMajorFreqs)
    {
        float x = jmap((float)std::log10(dFreq), 1.3f, 4.3f, 0.0f, fWidth);

        // Draw vertical grid line
        g.setColour(Colours::grey.withAlpha(0.3f));
        g.drawLine(x, 0, x, fHeight);
    }

    // Retrieve parameters
    double dTemperature = m_pcMainComponent.dGetParameter(PARAMETER_TEMPERATURE);
    double dHumidity = m_pcMainComponent.dGetParameter(PARAMETER_HUMIDITY);
    double dPressure = m_pcMainComponent.dGetParameter(PARAMETER_PRESSURE);
    double dDistance = m_pcMainComponent.dGetParameter(PARAMETER_DISTANCE);
    bool bMakeupGainEnabled = (bool)m_pcMainComponent.dGetParameter(PARAMETER_MAKEUP_GAIN);

    // Draw response curve
    for (int i = 0; i < 200; ++i)
    {
        double dFreq = 20.0 * std::pow(10.0, (i / 199.0) * 3.0); // Log scale: 20 Hz - 20 kHz

        // Calculate total attenuation
        double dAttenuationPerMeter = m_pcMainComponent.dCalculateAirAttenuationPerMetre(dFreq, dTemperature, dHumidity, dPressure);
        double dWindLoss =  -(float)Decibels::gainToDecibels(m_fpWindCoefficients->getMagnitudeForFrequency(dFreq, 44100.0));
        double dGradientLoss = -(float)Decibels::gainToDecibels(m_fpTempGradientCoefficients->getMagnitudeForFrequency(dFreq, 44100.0));
        double dTotalAttenuationDb = dAttenuationPerMeter * dDistance + (dWindLoss + dGradientLoss);

        if (!bMakeupGainEnabled)
        {
            double dAttentuationDueToDistance = 20 * log10(dDistance);
            dTotalAttenuationDb += dAttentuationDueToDistance;
        }

        //dTotalAttenuationDb = dWindLoss;

        // Map dB value to screen space (ensuring +10 dB is at top, -50 dB at bottom)
        float x = (float)i / 199.0f * fWidth;
        float y = jmap((float)-dTotalAttenuationDb, 10.0f, -50.0f, 0.0f, fHeight);

        if (i == 0)
            responseCurve.startNewSubPath(x, y);
        else 
            responseCurve.lineTo(x, y);
    }

    g.setColour(Colours::black);
    g.strokePath(responseCurve, PathStrokeType(2.0f));
}
