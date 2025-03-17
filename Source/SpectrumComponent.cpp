#include "SpectrumComponent.h"
#include "MainComponent.h"
#include "FrequencySpectrum.h"

/*======================================================================================*/
SpectrumComponent::SpectrumComponent(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    freqSpectrum = new FrequencySpectrum(480, 150);
    addAndMakeVisible(freqSpectrum);
    freqSpectrum->setOpaque(true);
}

/*======================================================================================*/
SpectrumComponent::~SpectrumComponent()
/*======================================================================================*/
{
    delete freqSpectrum;
}

/*======================================================================================*/
void SpectrumComponent::pushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    freqSpectrum->pushBuffer(buffer);
}

/*======================================================================================*/
void SpectrumComponent::resized()
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();
    Rectangle<int> rcSpectrumComponent = rect;
    freqSpectrum->setBounds(rect.getX() + 40, rect.getY(), 480, 150);
}
/*======================================================================================*/
void SpectrumComponent::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect(getLocalBounds().getX(), getLocalBounds().getY(), 40, getLocalBounds().getHeight());

    Colour clBackground = Colours::black;
    g.setColour(clBackground);
    g.fillRect(getLocalBounds()); // Fill the rectangle

    // Draw dB labels for the gradient
    g.setFont(10.0f);
    g.setColour(Colours::white);

    int dBValue = 10;

    int labelX = rect.getX() + 10; // Position for labels (just before the gradient)
    for (int y = rect.getY(); y <= rect.getY() + (rect.getHeight() - 20); y += ((rect.getHeight() - 20) / 5)) // 6 label positions (spread out)
    {
        // Draw label with dB value
        g.drawText(String(dBValue) + "dB", labelX, y, 30, 10, Justification::centredLeft);
        dBValue -= 10;
    }

    int labelY = rect.getY() + rect.getHeight() - 10; // Position for labels (just before the gradient)

    int start = rect.getX() + 30;
    int finish = rect.getX() + getLocalBounds().getWidth() - 30;

    // Define logarithmic scaling
    float minFreq = 20;  // Minimum frequency (e.g., 20 Hz)
    float maxFreq = 20000;  // Maximum frequency (e.g., 20 kHz)

    // Define the major frequencies that will be labeled
    std::vector<int> majorFrequencies = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 }; // Example major frequencies

    for (int i = 0; i < majorFrequencies.size(); ++i)
    {
        // Get the current major frequency
        int freq = majorFrequencies[i];

        // Calculate the position on the x-axis for this frequency
        
        float normalizedFreq = std::log10((float)freq / minFreq) / std::log10(maxFreq / minFreq);
        int x = start + (int)(normalizedFreq * (finish - start));

        // Format the label and display it
        if (freq >= 1000) 
        {
            // Use kHz for frequencies >= 1000 Hz
            g.drawText(String(freq / 1000.0f, 0) + "kHz", x, labelY, 30, 10, Justification::horizontallyCentred);
        }
        else 
        {
            // Use Hz for frequencies < 1000 Hz
            g.drawText(String(freq) + "Hz", x, labelY, 30, 10, Justification::horizontallyCentred);
        }
    }
}