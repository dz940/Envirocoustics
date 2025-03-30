#pragma once
#include <JuceHeader.h>

// For lapse
double dGetLowPassFilterCutoff(double dDistance)
{
    // Compute attenuation for lapse
    double A = 0.1 * (14.0 / 30.0);
    double dAttenuationdB = A * dDistance;
    double dCutoffFreq = 20000.0 * exp(-dAttenuationdB / 10.0);
    dCutoffFreq = juce::jmax(50.0, dCutoffFreq);
    return dCutoffFreq;
}

// For inversion
double dGetParametricBoost(double dDistance)
{
    double dBoostdB = 6.0 * (14.0 / 30.0) * log10(1 + (dDistance) / 300.0);
    dBoostdB = 1 + juce::jlimit(0.0, 12.0, dBoostdB); 
    return dBoostdB;
}

class TempGradientFilterCutoffSolver {
public:
    TempGradientFilterCutoffSolver(const bool bTemperatureInversion)
    {
        m_bIsTempInversion = bTemperatureInversion;
    }

    double SolveLapse(const double dDistance)
    {
        jassert(!m_bIsTempInversion);
        return dGetLowPassFilterCutoff(dDistance);
    }

    double SolveInversion(const double dDistance)
    {
        jassert(m_bIsTempInversion);
        return dGetParametricBoost(dDistance);
    }

private:

    bool m_bIsTempInversion;
};