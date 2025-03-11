#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class ResponseCurve : public Component
{
public:
    ResponseCurve(MainComponent& parentComponent);
    ~ResponseCurve();

    void setCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs);

    void paint(Graphics& g) override;

private:
    dsp::IIR::Coefficients<float>::Ptr coefficients;
    MainComponent& mainComponent;
};
