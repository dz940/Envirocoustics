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

private:
    MainComponent& m_pcMainComponent;
};
