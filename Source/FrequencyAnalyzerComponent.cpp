#include "FrequencyAnalyzerComponent.h"
#include "MainComponent.h"

FrequencyAnalyzerComponent::FrequencyAnalyzerComponent(MainComponent& parentComponent)
    : fftSize(2048),
    mainComponent(parentComponent)// Default FFT size
{
    // Initialize the FFT object with the desired FFT size
    fft = std::make_unique<juce::dsp::FFT>(std::log2(fftSize));

    // Resize the buffers for FFT processing and frequency magnitude storage
    fftData.resize(fftSize * 2, 0.0f);
    frequencyMagnitudes.resize(fftSize / 2, 0.0f);
    fftBuffer.resize(fftSize, 0.0f);

    // Start the timer to update the visualization every 60 FPS
    startTimerHz(60);
}

FrequencyAnalyzerComponent::~FrequencyAnalyzerComponent()
{
    // Stop the timer when the component is destroyed
    stopTimer();
}

void FrequencyAnalyzerComponent::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
    // Store the sample rate for later use
    this->sampleRate = sampleRate;
}

void FrequencyAnalyzerComponent::releaseResources()
{
    // Release any resources if needed when playback stops
}

void FrequencyAnalyzerComponent::processAudioBlock(const juce::AudioBuffer<float>& buffer)
{
    // Get the number of channels and samples in the buffer
    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();

    // Clear previous FFT data
    std::fill(fftData.begin(), fftData.end(), 0.0f);

    // Accumulate audio data from all channels
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        for (int i = 0; i < numSamples; ++i)
        {
            fftData[i] += channelData[i];
        }
    }

    // After accumulating the samples, perform the FFT
    if (numSamples > 0)
    {
        performFFT();
    }
}

void FrequencyAnalyzerComponent::performFFT()
{
    // Apply a windowing function (Hanning window) to reduce spectral leakage
    for (int i = 0; i < fftSize; ++i)
    {
        float window = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * i / (fftSize - 1)));
        fftData[i] *= window;
    }


    // Copy the accumulated data to the FFT buffer
    for (int i = 0; i < fftSize; ++i)
    {
        fftBuffer[i] = fftData[i];
    }

    juce::HeapBlock<float> fftData(fftSize * 2, true);
    juce::FloatVectorOperations::copy(fftData, fftBuffer.data(), fftSize);

    // Perform the FFT and store the result in fftBuffer
    fft->performFrequencyOnlyForwardTransform(fftBuffer.data());

    // Compute the magnitudes of the frequency bins
    for (int i = 0; i < fftSize / 2; ++i)
    {
        frequencyMagnitudes[i] = std::sqrt(fftBuffer[i] * fftBuffer[i]);
    }
}

void FrequencyAnalyzerComponent::paint(juce::Graphics& g)
{
    // Clear the background with a black color
    g.fillAll(juce::Colours::black);

    // Set the color for the frequency spectrum (lime green)
    g.setColour(juce::Colours::lime);

    // Get the number of bins in the frequency spectrum (half the FFT size)
    const int numBins = frequencyMagnitudes.size();

    // Draw the frequency spectrum as a line graph
    for (int i = 1; i < numBins; ++i)
    {
        // Map the frequency index to the x-axis (width of the component)
        float x1 = juce::jmap((float)i - 1, 0.0f, (float)numBins - 1, 0.0f, (float)getWidth());
        float x2 = juce::jmap((float)i, 0.0f, (float)numBins - 1, 0.0f, (float)getWidth());

        // Map the frequency magnitude to the y-axis (height of the component)
        float y1 = juce::jmap(frequencyMagnitudes[i - 1], 0.0f, 1000.0f, (float)getHeight(), 0.0f);
        float y2 = juce::jmap(frequencyMagnitudes[i], 0.0f, 1000.0f, (float)getHeight(), 0.0f);

        // Draw the line connecting the two points
        g.drawLine(x1, y1, x2, y2, 2.0f);
    }
}

void FrequencyAnalyzerComponent::resized()
{
    // Resize logic can be added here if needed
}

void FrequencyAnalyzerComponent::timerCallback()
{
    // This method is called every 60 FPS to repaint the component
    repaint();
}
