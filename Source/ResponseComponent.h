#pragma once
#include <JuceHeader.h>

class ResponseCurve;
class MainComponent;
using namespace juce;

class ResponseComponent : public Component
{
public:
    ResponseComponent(MainComponent& parentComponent);
    ~ResponseComponent();
    void resized() override;
    void paint(Graphics& g) override;

    void vSetWindCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs);
    void vSetTempGradientCoefficients(dsp::IIR::Coefficients<float>::Ptr newCoeffs);

private:
    ResponseCurve* m_pcResponseCurve;
    MainComponent& m_pcMainComponent;
};
