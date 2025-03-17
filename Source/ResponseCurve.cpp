#include "ResponseCurve.h"
#include "MainComponent.h"

/*======================================================================================*/
ResponseCurve::ResponseCurve(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{

}

/*======================================================================================*/
ResponseCurve::~ResponseCurve()
/*======================================================================================*/
{
}

/*======================================================================================*/
void ResponseCurve::setCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs)
/*======================================================================================*/
{
    coefficients = newCoeffs;
    repaint();
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
    g.setColour(juce::Colours::black);
    g.setFont(20.0f);

    // Title
    g.drawText("System Response Curve", 0, 0, getWidth(), 30, juce::Justification::centred, true);

    // Define drawing bounds
    float width = (float)getWidth();
    float height = (float)getHeight();

    float topOffset = 30.0f;        // 30px for title
    float bottomOffset = 15.0f;     // 10px space at bottom for labels
    float leftMargin = 35.0f;       // Reduced left margin, still enough for dB labels
    float rightMargin = 15.0f;      // Ensure space on the right
    float graphWidth = width - leftMargin - rightMargin;
    float graphHeight = height - topOffset - bottomOffset;

    juce::Path responseCurve;
    g.setFont(10.0f); // Smaller font for labels

    // Draw Magnitude Labels (+10 dB at top, -50 dB at bottom)
    for (int dB = 10; dB >= -50; dB -= 10)
    {
        float y = topOffset + juce::jmap((float)dB, 10.0f, -50.0f, 0.0f, graphHeight);

        g.setColour(juce::Colours::black);
        g.drawText(juce::String(dB) + " dB", 0, y - 5, 30, 10, juce::Justification::right, false);

        // Draw horizontal grid line
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.drawLine(leftMargin, y, width - rightMargin, y);
    }

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> majorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };

    for (double freq : majorFreqs)
    {
        float x = leftMargin + juce::jmap((float)std::log10(freq), 1.3f, 4.3f, 0.0f, graphWidth);

        g.setColour(juce::Colours::black);
        if (freq < 1000)
        {
            g.drawText(juce::String((int)freq) + "Hz", x - 15, height - 15, 30, 15, juce::Justification::centred, false);
        }
        else
        {
            int newFreq = freq / 1000;
            g.drawText(juce::String((int)newFreq) + "kHz", x - 15, height - 15, 30, 15, juce::Justification::centred, false);
        }

        // Draw vertical grid line
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.drawLine(x, topOffset, x, height - bottomOffset);
    }

    // Retrieve parameters
    double temperature = mainComponent.nGetParameter(PARAMETER_TEMPERATURE);
    double humidity = mainComponent.nGetParameter(PARAMETER_HUMIDITY);
    double pressure = mainComponent.nGetParameter(PARAMETER_PRESSURE);
    double distance = mainComponent.nGetParameter(PARAMETER_DISTANCE);
    double windSpeed = mainComponent.nGetParameter(PARAMETER_WIND_SPEED);
    bool windDirection = mainComponent.nGetParameter(PARAMETER_WIND_DIRECTION);
    int temperatureGradient = mainComponent.nGetParameter(PARAMETER_TEMP_GRADIENT);

    // Draw response curve
    for (int i = 0; i < 200; ++i)
    {
        double freq = 20.0 * std::pow(10.0, (i / 199.0) * 3.0); // Log scale: 20 Hz - 20 kHz

        // Calculate total attenuation
        double attenuationPerMeter = mainComponent.dCalculateAirAttenuationPerMetre(freq, temperature, humidity, pressure);
        double windLoss = mainComponent.dCalculateWindLoss(freq, distance, temperature, windSpeed, windDirection);
        double gradientLoss = mainComponent.dCalculateTemperatureGradientLoss(freq, distance, temperatureGradient);
        double totalAttenuationDb = attenuationPerMeter * distance + windLoss + gradientLoss;

        // Map dB value to screen space (ensuring +10 dB is at top, -50 dB at bottom)
        float x = leftMargin + (float)i / 199.0f * graphWidth;
        float y = topOffset + juce::jmap((float)-totalAttenuationDb, 10.0f, -50.0f, 0.0f, graphHeight);

        if (i == 0)
            responseCurve.startNewSubPath(x, y);
        else if( y < topOffset + graphHeight)
            responseCurve.lineTo(x, y);
        else
            responseCurve.lineTo(x, topOffset + graphHeight);
    }

    g.setColour(juce::Colours::blue);
    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
}
