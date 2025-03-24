#include "WaveformDisplay.h"
#include "MainComponent.h"

/*======================================================================================*/
WaveformDisplay::WaveformDisplay(MainComponent& pcParentComponent)
    : m_pcMainComponent(pcParentComponent)
/*======================================================================================*/
{
    m_cVisualiser.setBufferSize(128); // Number of samples per visual segment
    m_cVisualiser.setSamplesPerBlock(16); // Number of samples to process per block
    m_cVisualiser.setColours(Colours::darkseagreen, Colours::black); // Background and waveform colors
    m_cVisualiser.setNumChannels(1);
    addAndMakeVisible(m_cVisualiser);
}

/*======================================================================================*/
WaveformDisplay::~WaveformDisplay()
/*======================================================================================*/
{
}

/*======================================================================================*/
void WaveformDisplay::vPushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    // Create a mono buffer by averaging the channels
    AudioBuffer<float> monoBuffer(1, buffer.getNumSamples()); // Single channel buffer
    monoBuffer.clear(); // Clear the buffer to initialize with zeros

    for (int nChannel = 0; nChannel < buffer.getNumChannels(); ++nChannel)
    { monoBuffer.addFrom(0, 0, buffer, nChannel, 0, buffer.getNumSamples(), 1.0f / buffer.getNumChannels()); }

    m_cVisualiser.pushBuffer(buffer); // Forward the buffer to the visualizer
}

/*======================================================================================*/
void WaveformDisplay::resized()
/*======================================================================================*/
{
    m_cVisualiser.setBounds(getLocalBounds()); // Make the visualizer fill the component
}

/*======================================================================================*/
void WaveformDisplay::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    g.setColour(Colours::white); // Border color
    g.drawRect(rcRect.expanded(2), 2); // Border thickness
}