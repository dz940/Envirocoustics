#include "WaveformDisplay.h"
#include "MainComponent.h"

/*======================================================================================*/
WaveformDisplay::WaveformDisplay(MainComponent& pcParentComponent)
    : m_pcMainComponent(pcParentComponent)
/*======================================================================================*/
{
    m_pcVisualiser = new AudioVisualiserComponent(1);
    addAndMakeVisible(m_pcVisualiser);
    m_pcVisualiser->setOpaque(true);

    m_pcVisualiser->setBufferSize(128); // Number of samples per visual segment
    m_pcVisualiser->setSamplesPerBlock(16); // Number of samples to process per block
    juce::Colour clBackGroundColour = juce::Colour(COLOUR_COMPONENT_BACKGROUND);
    m_pcVisualiser->setColours(clBackGroundColour, Colours::black); // Background and waveform colors
    m_pcVisualiser->setNumChannels(1);
}

/*======================================================================================*/
WaveformDisplay::~WaveformDisplay()
/*======================================================================================*/
{
    delete m_pcVisualiser;
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

    m_pcVisualiser->pushBuffer(buffer); // Forward the buffer to the visualizer
}

/*======================================================================================*/
void WaveformDisplay::resized()
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    rcRect.reduce(2, 2);
    m_pcVisualiser->setBounds(rcRect); // Make the visualizer fill the component
}

/*======================================================================================*/
void WaveformDisplay::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    Rectangle<float> rcWaveform = rcRect.toFloat(); // x, y, width, height

    juce::Colour clOutlineColour = juce::Colour(COLOUR_COMPONENT_OUTLINE);
    g.setColour(clOutlineColour);
    g.drawRect(rcWaveform, 2);
}