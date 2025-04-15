#include "SpectrumComponent.h"
#include "MainComponent.h"
#include "FrequencySpectrum.h"

/*======================================================================================*/
SpectrumComponent::SpectrumComponent(MainComponent& pcParentComponent)
    : m_pcMainComponent(pcParentComponent)
/*======================================================================================*/
{
    m_pcFreqSpectrum = new FrequencySpectrum(465, 150);
    addAndMakeVisible(m_pcFreqSpectrum);
    m_pcFreqSpectrum->setOpaque(true);
}

/*======================================================================================*/
SpectrumComponent::~SpectrumComponent()
/*======================================================================================*/
{
    delete m_pcFreqSpectrum;
}

/*======================================================================================*/
void SpectrumComponent::vPushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    m_pcFreqSpectrum->vPushBuffer(buffer);
}

/*======================================================================================*/
void SpectrumComponent::resized()
/*======================================================================================*/
{
    Rectangle<int> rcSpectrumComponent = getLocalBounds();
    m_pcFreqSpectrum->setBounds(rcSpectrumComponent.getX() + 40, rcSpectrumComponent.getY(), 465, 150);
}
/*======================================================================================*/
void SpectrumComponent::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rcRect(getLocalBounds().getX(), getLocalBounds().getY(), 40, getLocalBounds().getHeight());

    Colour clBackground = Colours::black;
    g.setColour(clBackground);
    g.fillRect(getLocalBounds()); // Fill the rectangle

    // Draw dB labels for the gradient
    g.setFont(10.0f);
    g.setColour(Colours::white);

    int ndBValue = 50;

    int nLabelX = rcRect.getX() + 10; // Position for labels (just before the gradient)
    for (int y = rcRect.getY(); y <= rcRect.getY() + (rcRect.getHeight() - 20); y += ((rcRect.getHeight() - 20) / 5)) // 6 label positions (spread out)
    {
        // Draw label with dB value
        g.drawText(String(ndBValue) + "dB", nLabelX, y, 30, 10, Justification::centredLeft);
        ndBValue -= 10;
    }

    int nLabelY = rcRect.getY() + rcRect.getHeight() - 10; // Position for labels (just before the gradient)

    int nStart = getLocalBounds().getX() + 40;
    int nFinish = getLocalBounds().getX() + 40 + (getLocalBounds().getWidth() - 55);

    // Define logarithmic scaling
    float fMinFreq = 20;  // Minimum frequency (e.g., 20 Hz)
    float fMaxFreq = 20000;  // Maximum frequency (e.g., 20 kHz)

    // Define the major frequencies that will be labeled
    std::vector<int> nMajorFrequencies = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 }; // Example major frequencies

    for (int i = 0; i < nMajorFrequencies.size(); ++i)
    {
        // Get the current major frequency
        int nFreq = nMajorFrequencies[i];

        // Calculate the position on the x-axis for this frequency
        
        float fNormalizedFreq = std::log10((float)nFreq / fMinFreq) / std::log10(fMaxFreq / fMinFreq);
        int x = nStart + (int)(fNormalizedFreq * (nFinish - nStart));

        // Format the label and display it
        if (nFreq >= 1000)
        {
            // Use kHz for frequencies >= 1000 Hz
            g.drawText(String(nFreq / 1000.0f, 0) + "kHz", x - 15, nLabelY, 30, 10, Justification::horizontallyCentred);
        }
        else 
        {
            // Use Hz for frequencies < 1000 Hz
            g.drawText(String(nFreq) + "Hz", x-15, nLabelY, 30, 10, Justification::horizontallyCentred);
        }
    }
}