#include "SpectrogramComponent.h"
#include "MainComponent.h"
#include "Spectrogram.h"

/*======================================================================================*/
SpectrogramComponent::SpectrogramComponent(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    spectrogram = new Spectrogram(430, 160);
    addAndMakeVisible(spectrogram);
    spectrogram->setOpaque(true);
}

/*======================================================================================*/
SpectrogramComponent::~SpectrogramComponent()
/*======================================================================================*/
{
    delete spectrogram;
}

/*======================================================================================*/
void SpectrogramComponent::pushBuffer(const juce::AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    spectrogram->pushBuffer(buffer);
}

/*======================================================================================*/
void SpectrogramComponent::resized()
/*======================================================================================*/
{
    juce::Rectangle rect = getLocalBounds();
    juce::Rectangle<int> rcSpectrogramComponent = rect;

    spectrogram->setBounds(rect.getX() + 40, rect.getY(), 430, 160);

}
/*======================================================================================*/
void SpectrogramComponent::paint(juce::Graphics& g)
/*======================================================================================*/
{
    juce::Rectangle rect(getLocalBounds().getX() + getLocalBounds().getWidth() - 50, getLocalBounds().getY(), 50, getLocalBounds().getHeight());

    juce::Colour clBackground = juce::Colour::fromHSV(0.67f, 1.0f, 1.0f, 1.0f);
    g.setColour(clBackground);
    juce::Rectangle<int> rcGradientKey = rect; // x, y, width, height
    g.fillRect(getLocalBounds()); // Fill the rectangle

    int keyStartX = rect.getX() + 40; // Positioning the gradient
    int keyEndX = rect.getX() + 50;

    for (int y = 0; y < rect.getHeight(); ++y)
    {
        float magnitude = 1.0f - (float)y / (rect.getHeight()); // Normalize magnitude
        float hue = juce::jlimit(0.0f, 0.67f, 0.67f - (magnitude * 0.67f)); // Mapping to color range
        juce::Colour color = juce::Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f);

        for (int x = keyStartX; x < keyEndX; ++x)
        {
            g.setColour(color); // Set the color for the gradient line
            g.drawLine((float)x, (float)(y + rect.getY()), (float)(x + 1), (float)(y + rect.getY())); // Draw a 1px line to form the gradient
        }
    }

    // Draw dB labels for the gradient
    g.setFont(10.0f);
    g.setColour(juce::Colours::white);

    int dBValue = 10;

    int labelX = rect.getX() + 10; // Position for labels (just before the gradient)
    for (int y = rect.getY(); y <= rect.getY() + (rect.getHeight() - 10); y += ((rect.getHeight() - 10) / 5)) // 6 label positions (spread out)
    {
        // Draw label with dB value
        g.drawText(juce::String(dBValue) + "dB", labelX, y, 30, 10, juce::Justification::centred);
        dBValue -= 10;
    }
 
    // Define the linear scaling for the vertical axis
    const float minFreq = 0.0f;    // Minimum frequency (e.g., 0 Hz)
    const float maxFreq = 20000.0f; // Maximum frequency (e.g., 20 kHz)

    // Define the major frequencies for fewer labels, linearly spaced
    std::vector<int> majorFrequencies = { 20, 5000, 10000, 15000, 20000 }; // Linearly spaced frequencies

    labelX = getLocalBounds().getX();  // Position for labels (just before the gradient)

    // Loop through the major frequencies to position the labels
    for (int i = 0; i < majorFrequencies.size(); ++i)
    {
        // Get the current major frequency
        int freq = majorFrequencies[i];

        // Calculate the position on the y-axis for this frequency (linearly mapped)
        // Normalize the frequency from 0 Hz to 20 kHz to the range [0, 1]
        float normalizedFreq = (float)(freq - minFreq) / (maxFreq - minFreq);

        // Adjust the 'y' position based on the normalized frequency
        // Ensure that 0 Hz is placed at the topmost visible position
        int y;
        if (freq == 20) 
        {
            // Place 0 Hz at the top of the spectrogram area (or a little below)
            y = getLocalBounds().getY() + getLocalBounds().getHeight() - 10; // Adding a margin of 10 pixels for visibility
        }
        else 
        {
            // Map this normalized value to the height of the spectrogram
            y = getLocalBounds().getY() + getLocalBounds().getHeight() - (int)(normalizedFreq * getLocalBounds().getHeight());
        }

        // Format the label and display it
        if (freq != 20) 
        {
            // Use kHz for frequencies >= 1000 Hz
            g.drawText(juce::String(freq / 1000.0f, 0) + "kHz", labelX, y, 30, 10, juce::Justification::centredRight);
        }
        else 
        {
            // Use Hz for frequencies < 1000 Hz
            g.drawText(juce::String(freq) + "Hz", labelX, y, 30, 10, juce::Justification::centredRight);
        }
    }

    gradientDrawn = true; // Mark the gradient as drawn
    
}