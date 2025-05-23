#include "SpectrogramComponent.h"
#include "MainComponent.h"
#include "Spectrogram.h"

/*======================================================================================*/
SpectrogramComponent::SpectrogramComponent(MainComponent& pcParentComponent)
    : m_pcMainComponent(pcParentComponent)
/*======================================================================================*/
{
    m_pcSpectrogram = new Spectrogram(430, 160);
    addAndMakeVisible(m_pcSpectrogram);
    m_pcSpectrogram->setOpaque(true);
}

/*======================================================================================*/
SpectrogramComponent::~SpectrogramComponent()
/*======================================================================================*/
{
    delete m_pcSpectrogram;
}

/*======================================================================================*/
void SpectrogramComponent::vPushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    m_pcSpectrogram->vPushBuffer(buffer);
}

/*======================================================================================*/
void SpectrogramComponent::resized()
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    m_pcSpectrogram->setBounds(rcRect.getX() + 40, rcRect.getY(), 430, 160);
}

/*======================================================================================*/
void SpectrogramComponent::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rcRect(getLocalBounds().getX() + getLocalBounds().getWidth() - 50, getLocalBounds().getY(), 50, getLocalBounds().getHeight());

    Colour clBackground = Colour::fromHSV(0.67f, 1.0f, 1.0f, 1.0f);
    g.setColour(clBackground);
    g.fillRect(getLocalBounds()); // Fill the rectangle

    int nKeyStartX = rcRect.getX() + 40; // Positioning the gradient
    int nKeyEndX = rcRect.getX() + 50;

    for (int y = 0; y < rcRect.getHeight(); ++y)
    {
        float fMagnitude = 1.0f - (float)y / (rcRect.getHeight()); // Normalize magnitude
        float fHue = jlimit(0.0f, 0.67f, 0.67f - (fMagnitude * 0.67f)); // Mapping to color range
        Colour color = Colour::fromHSV(fHue, 1.0f, 1.0f, 1.0f);

        for (int x = nKeyStartX; x < nKeyEndX; ++x)
        {
            g.setColour(color); // Set the color for the gradient line
            g.drawLine((float)x, (float)(y + rcRect.getY()), (float)(x + 1), (float)(y + rcRect.getY())); // Draw a 1px line to form the gradient
        }
    }

    // Draw dB labels for the gradient
    g.setFont(10.0f);
    g.setColour(Colours::white);

    int ndBValue = 50;

    int nLabelX = rcRect.getX() + 10; // Position for labels (just before the gradient)
    for (int y = rcRect.getY(); y <= rcRect.getY() + (rcRect.getHeight() - 10); y += ((rcRect.getHeight() - 10) / 5)) // 6 label positions (spread out)
    {
        // Draw label with dB value
        g.drawText(String(ndBValue) + "dB", nLabelX, y, 30, 10, Justification::centred);
        ndBValue -= 10;
    }
 
    // Define the linear scaling for the vertical axis
    const float fMinFreq = 20.0f;    
    const float fMaxFreq = 15000.0f;

    // Define the major frequencies for fewer labels, linearly spaced
    std::vector<int> nMajorFrequencies = { 20, 5000, 10000, 15000 }; // Linearly spaced frequencies

    nLabelX = getLocalBounds().getX();  // Position for labels (just before the gradient)

    // Loop through the major frequencies to position the labels
    for (int i = 0; i < nMajorFrequencies.size(); ++i)
    {
        // Get the current major frequency
        int nFreq = nMajorFrequencies[i];

        // Calculate the position on the y-axis for this frequency (linearly mapped)
        // Normalize the frequency from 0 Hz to 20 kHz to the range [0, 1]
        float fNormalizedFreq = (float)(nFreq - fMinFreq) / (fMaxFreq - fMinFreq);

        // Adjust the 'y' position based on the normalized frequency
        // Ensure that 0 Hz is placed at the topmost visible position
        int y;
        if (nFreq == 20)
        {
            // Place 0 Hz at the top of the spectrogram area (or a little below)
            y = getLocalBounds().getY() + getLocalBounds().getHeight() - 10; // Adding a margin of 10 pixels for visibility
            g.drawText(String(nFreq) + "Hz", nLabelX, y, 30, 10, Justification::centredRight);
        }
        else 
        {
            // Map this normalized value to the height of the spectrogram
            y = getLocalBounds().getY() + getLocalBounds().getHeight() - (int)(fNormalizedFreq * getLocalBounds().getHeight());
            g.drawText(String(nFreq / 1000.0f, 0) + "kHz", nLabelX, y, 30, 10, Justification::centredRight);
        }
    }
}