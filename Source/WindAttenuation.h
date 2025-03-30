#pragma once
#include <JuceHeader.h>

// For upwind 
double dGetLowPassFilterCutoff(double dWindSpeed, double dDistance)
{
    // Compute attenuation for upwind
    double A = 0.1 * (dWindSpeed / 30.0);
    double dAttenuationdB = A * dDistance;
    double dCutoffFreq = 20000.0 * expf(-dAttenuationdB / 10.0);
    dCutoffFreq = juce::jmax(50.0, dCutoffFreq);
    return dCutoffFreq;
}

// For downwind
double dGetHighShelfBoost(double dWindSpeed, double dDistance)
{
    double dBoostdB = 6.0 * (dWindSpeed / 30.0) * log10(1 + (dDistance) / 300.0);
    dBoostdB = 1 + juce::jlimit(0.001, 12.0, dBoostdB);  // allows up to 12 dB for hurricane winds
    return dBoostdB;
}

class WindFilterCutoffSolver {
public:
    WindFilterCutoffSolver(const double dWindSpeed,
        const bool bIsDownwind)
    { 
        // Convert wind speed to m/s
        m_dWindSpeed = dWindSpeed/3.6;
        m_bIsDownwind = bIsDownwind;
    }

    double SolveUpwind(const double dDistance)
    {
        jassert(!m_bIsDownwind);
        return dGetLowPassFilterCutoff(m_dWindSpeed,dDistance);
    }

    double SolveDownwind(const double dDistance)
    {
        jassert(m_bIsDownwind);
        return dGetHighShelfBoost(m_dWindSpeed, dDistance);
    }

private:

    double m_dWindSpeed;
    bool m_bIsDownwind;
};