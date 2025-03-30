#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class ResponseCurve : public Component
{
public:
    ResponseCurve(MainComponent& pcParentComponent);
    ~ResponseCurve();

    void paint(Graphics& g) override;

    void vSetWindCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs)
    { m_fpWindCoefficients = newCoeffs; }
    void vSetTempGradientCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs)
    { m_fpTempGradientCoefficients = newCoeffs; }  

private:
    MainComponent& m_pcMainComponent;
    dsp::IIR::Coefficients<float>::Ptr m_fpWindCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(44100, 22000);
    dsp::IIR::Coefficients<float>::Ptr m_fpTempGradientCoefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(44100, 22000);
};
