#include "WaveformDisplay.h"
#include "MainComponent.h"

/*======================================================================================*/
WaveformDisplay::WaveformDisplay(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    visualiser.setBufferSize(128); // Number of samples per visual segment
    visualiser.setSamplesPerBlock(16); // Number of samples to process per block
    visualiser.setColours(Colours::darkseagreen, Colours::black); // Background and waveform colors
    visualiser.setNumChannels(1);
    addAndMakeVisible(visualiser);
}

/*======================================================================================*/
WaveformDisplay::~WaveformDisplay()
/*======================================================================================*/
{
}

/*======================================================================================*/
void WaveformDisplay::pushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    // Create a mono buffer by averaging the channels
    AudioBuffer<float> monoBuffer(1, buffer.getNumSamples()); // Single channel buffer
    monoBuffer.clear(); // Clear the buffer to initialize with zeros

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    { monoBuffer.addFrom(0, 0, buffer, channel, 0, buffer.getNumSamples(), 1.0f / buffer.getNumChannels()); }

    visualiser.pushBuffer(buffer); // Forward the buffer to the visualizer
}

/*======================================================================================*/
void WaveformDisplay::resized()
/*======================================================================================*/
{
    visualiser.setBounds(getLocalBounds()); // Make the visualizer fill the component
}

/*======================================================================================*/
void WaveformDisplay::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();
    g.setColour(Colours::white); // Border color
    g.drawRect(rect.expanded(2), 2); // Border thickness
}