#include "ResponseCurve.h"
#include "MainComponent.h"

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


///*======================================================================================*/
//void ResponseCurve::paint(Graphics& g) 
///*======================================================================================*/
//{
//    g.fillAll(Colours::darkseagreen);
//    g.setColour(Colours::black);
//    g.setFont(20.0f);
//    g.drawText("System response curves", getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getWidth(), 30, Justification::centred, true);
//
//    if (coefficients == nullptr)
//        return;
//
//    Path magnitudeCurve;
//    Path phaseCurve;
//
//    float width = (float)getWidth();
//    float height = (float)getHeight();
//
//    float magnitudePlotHeight = (height - 30);  // 60% of height for magnitude
//    float phasePlotHeight = (height - 30);      // 40% of height for phase      // Start phase plot below magnitude
//
//    for (int i = 0; i < 200; ++i) // 200 points for smooth curve
//    {
//        double freq = 20.0 * std::pow(10.0, (i / 199.0) * 3.0); // Log scale: 20 Hz - 20 kHz
//
//        // Compute Magnitude Response (in dB)
//        float magnitudeDb = (float)Decibels::gainToDecibels(coefficients->getMagnitudeForFrequency(freq, 44100.0));
//
//        // Compute Phase Response (in degrees)
//        float phaseDeg = coefficients->getPhaseForFrequency(freq, 44100.0) * (180.0f / float_Pi); // Convert to degrees
//
//        // Normalize positions for drawing
//        float x = (float)i / 199.0f * width;
//
//        float magnitudeY = 30 + jmap(magnitudeDb, -60.0f, 0.0f, magnitudePlotHeight, 0.0f); // Map magnitude to top 60%
//        float phaseY = 30 + jmap(phaseDeg, -180.0f, 180.0f, phasePlotHeight, 0.0f); // Map phase from -180 to 180 degrees
//
//        // Draw Magnitude Curve
//        if (i == 0)
//            magnitudeCurve.startNewSubPath(x, magnitudeY);
//        else
//            magnitudeCurve.lineTo(x, magnitudeY);
//
//        // Draw Phase Curve
//        if (i == 0)
//            phaseCurve.startNewSubPath(x, phaseY);
//        else
//            phaseCurve.lineTo(x, phaseY);
//    }
//
//    // Draw the magnitude response in BLUE
//    g.setColour(Colours::blue);
//    g.strokePath(magnitudeCurve, PathStrokeType(2.0f));
//
//    // Draw the phase response in RED
//    g.setColour(Colours::red);
//    g.strokePath(phaseCurve, PathStrokeType(2.0f));
//}

///*======================================================================================*/
//void ResponseCurve::paint(juce::Graphics& g)
///*======================================================================================*/
//{
//    g.fillAll(juce::Colours::darkseagreen);
//    g.setColour(juce::Colours::black);
//    g.setFont(20.0f);
//    g.drawText("System Response Curve", getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getWidth(), 30, juce::Justification::centred, true);
//
//    juce::Path responseCurve;
//    float width = (float)getWidth();
//    float height = (float)getHeight();
//
//    // Air attenuation parameters (can be user-defined later)
//    double temperature = mainComponent.nGetParameter(PARAMETER_TEMPERATURE);   // Degrees Celsius
//    double humidity = mainComponent.nGetParameter(PARAMETER_HUMIDITY);      // Percentage
//    double pressure = mainComponent.nGetParameter(PARAMETER_PRESSURE);
//    double distance = mainComponent.nGetParameter(PARAMETER_DISTANCE);      // Distance in meters
//    double windSpeed = mainComponent.nGetParameter(PARAMETER_WIND_SPEED);
//    bool windDirection = mainComponent.nGetParameter(PARAMETER_WIND_DIRECTION);
//    int temperatureGradient = mainComponent.nGetParameter(PARAMETER_TEMP_GRADIENT);
//
//    for (int i = 0; i < 200; ++i)
//    {
//        double freq = 20.0 * std::pow(10.0, (i / 199.0) * 3.0); // Log scale: 20 Hz - 20 kHz
//
//        // Calculate air attenuation in dB per meter
//        double attenuationPerMeter = mainComponent.dCalculateAirAttenuationPerMetre(freq, temperature, humidity, pressure);
//        double windLoss = mainComponent.dCalculateWindLoss(freq, distance, temperature, windSpeed, windDirection); 
//        double gradientLoss = mainComponent.dCalculateTemperatureGradientLoss(freq, distance, temperatureGradient);
//        double totalAttenuationDb = attenuationPerMeter * distance + windLoss + gradientLoss; // Loss over given distance
//
//        // Normalize to graph height (-60 dB to 0 dB range)
//        float x = (float)i / 199.0f * width;
//        float y = 30 + juce::jmap((float)-totalAttenuationDb, -30.0f, 10.0f, height - 30, 0.0f);
//
//        if (i == 0)
//            responseCurve.startNewSubPath(x, y);
//        else
//            responseCurve.lineTo(x, y);
//    }
//
//    g.setColour(juce::Colours::blue);
//    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
//}

/*======================================================================================*/
void ResponseCurve::paint(juce::Graphics& g)
/*======================================================================================*/
{
    g.fillAll(juce::Colours::darkseagreen);

    // Define drawing bounds
    float fWidth = (float)getWidth();
    float fHeight = (float)getHeight();

    juce::Path responseCurve;
    g.setFont(10.0f); // Smaller font for labels

    // Draw horizontal grid lines
    for (int ndB = 10; ndB >= -50; ndB -= 10)
    {
        float y = juce::jmap((float)ndB, 10.0f, -50.0f, 0.0f, fHeight);

        // Draw horizontal grid line
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.drawLine(0, y, fWidth, y);
    }

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> dMajorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };

    for (double dFreq : dMajorFreqs)
    {
        float x = juce::jmap((float)std::log10(dFreq), 1.3f, 4.3f, 0.0f, fWidth);

        // Draw vertical grid line
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.drawLine(x, 0, x, fHeight);
    }

    // Retrieve parameters
    double dTemperature = m_pcMainComponent.nGetParameter(PARAMETER_TEMPERATURE);
    double dHumidity = m_pcMainComponent.nGetParameter(PARAMETER_HUMIDITY);
    double dPressure = m_pcMainComponent.nGetParameter(PARAMETER_PRESSURE);
    double dDistance = m_pcMainComponent.nGetParameter(PARAMETER_DISTANCE);
    double dWindSpeed = m_pcMainComponent.nGetParameter(PARAMETER_WIND_SPEED);
    bool bWindDirection = m_pcMainComponent.nGetParameter(PARAMETER_WIND_DIRECTION);
    int nTemperatureGradient = m_pcMainComponent.nGetParameter(PARAMETER_TEMP_GRADIENT);

    // Draw response curve
    for (int i = 0; i < 200; ++i)
    {
        double dFreq = 20.0 * std::pow(10.0, (i / 199.0) * 3.0); // Log scale: 20 Hz - 20 kHz

        // Calculate total attenuation
        double dAttenuationPerMeter = m_pcMainComponent.dCalculateAirAttenuationPerMetre(dFreq, dTemperature, dHumidity, dPressure);
        double dWindLoss = m_pcMainComponent.dCalculateWindLoss(dFreq, dDistance, dWindSpeed, bWindDirection);
        double dGradientLoss = m_pcMainComponent.dCalculateTemperatureGradientLoss(dFreq, dDistance, nTemperatureGradient);
        double dTotalAttenuationDb = dAttenuationPerMeter * dDistance + dWindLoss + dGradientLoss;

        // Map dB value to screen space (ensuring +10 dB is at top, -50 dB at bottom)
        float x = (float)i / 199.0f * fWidth;
        float y = juce::jmap((float)-dTotalAttenuationDb, 10.0f, -50.0f, 0.0f, fHeight);

        if (i == 0)
            responseCurve.startNewSubPath(x, y);
        else 
            responseCurve.lineTo(x, y);
    }

    g.setColour(juce::Colours::blue);
    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
}
