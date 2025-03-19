#include "FrequencySpectrum.h"
#include "MainComponent.h"

/*======================================================================================*/
FrequencySpectrum::FrequencySpectrum(int nWidth, int nHeight)
    : frequencySpectrumImage(Image::ARGB, nWidth, nHeight, true)
/*======================================================================================*/
{
    fifo.setSize(1, fftSize); // One-channel buffer
}

/*======================================================================================*/
FrequencySpectrum::~FrequencySpectrum() 
/*======================================================================================*/
{
}

/*======================================================================================*/
void FrequencySpectrum::pushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    auto* channelData = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        fifo.getWritePointer(0)[fifoIndex++] = channelData[i];

        if (fifoIndex >= fftSize)
        {
            fifoIndex = 0;
            drawNextFrameOfFrequencySpectrum();
        }
    }
}

/*======================================================================================*/
void FrequencySpectrum::applyDecay()
/*======================================================================================*/
{
    float decayFactor = 0.1f;
    Graphics g(frequencySpectrumImage);
    g.setColour(Colours::black.withAlpha(1.0f - decayFactor));
    g.fillRect(frequencySpectrumImage.getBounds()); // Fades out the old image
}

/*======================================================================================*/
void FrequencySpectrum::drawNextFrameOfFrequencySpectrum()
/*======================================================================================*/
{
    const MessageManagerLock mmLock;
    if (!mmLock.lockWasGained()) return;

    HeapBlock<float> fftData(fftSize * 2, true);
    FloatVectorOperations::copy(fftData, fifo.getReadPointer(0), fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);
    
    applyDecay();

    auto width = frequencySpectrumImage.getWidth();
    auto height = frequencySpectrumImage.getHeight();

    Path frequencyCurve;

    float minBin = 1.0f;
    float maxBin = fftSize / 2;
    float logMin = std::log10(minBin);
    float logMax = std::log10(maxBin);

    float prevX = 0, prevY = (float)height;

    frequencyCurve.startNewSubPath(0, (float)height); // **Anchor the path to bottom-left**

    for (int x = 0; x < width; ++x)
    {
        float normX = (float)x / width;
        float logBin = logMin + normX * (logMax - logMin);
        float binPos = std::pow(10.0f, logBin);

        int binIndex = jlimit(0, (fftSize / 2) - 1, (int)binPos);
        int nextBinIndex = jlimit(0, (fftSize / 2) - 1, binIndex + 1);
        float binFraction = binPos - binIndex;

        float magnitude1 = fftData[binIndex];
        float magnitude2 = fftData[nextBinIndex];

        float magnitude = magnitude1 * (1.0f - binFraction) + magnitude2 * binFraction;
        magnitude = Decibels::gainToDecibels(magnitude + 1e-6f);
        magnitude = jmap(magnitude, 0.0f, 50.0f, 0.0f, 1.0f);
        magnitude = jlimit(0.0f, 1.0f, magnitude);

        int yPos = (int)(height - 1 - (magnitude * height));

        if (x == 0)
            frequencyCurve.lineTo((float)x, (float)yPos); // Start connecting from (0, yPos)
        else
            frequencyCurve.quadraticTo(prevX, prevY, (float)x, (float)yPos); // Smooth transition

        prevX = (float)x;
        prevY = (float)yPos;
    }

    frequencyCurve.lineTo((float)width, (float)height); // **Close at bottom-right**
    frequencyCurve.lineTo(0, (float)height);     // **Return to bottom-left**
    frequencyCurve.closeSubPath();        // **Ensure a fully enclosed fill**

    // **Apply a Vertical Gradient**
    ColourGradient gradient(
        Colours::cyan.withAlpha(0.9f), (float)(width / 2), 0,
        Colours::blue.withAlpha(0.3f), (float)(width / 2), (float)height,
        false
    );

    Graphics g(frequencySpectrumImage);
    g.setGradientFill(gradient);
    g.fillPath(frequencyCurve); // **Now properly enclosed and filled!**

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> majorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
    float logMinFreq = std::log10(20.0f);
    float logMaxFreq = std::log10(20000.0f);
    for (double freq : majorFreqs)
    {
        float x = jmap((float)std::log10(freq), logMinFreq, logMaxFreq, 0.0f, (float)width);

        // Draw vertical grid line
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.drawLine(x, 0, x, height);
    }

    repaint();
}

/*======================================================================================*/
void FrequencySpectrum::paint(Graphics& g)
/*======================================================================================*/
{
    // Get the position and size of the Spectrogram component
    Rectangle<float> rcImage(getLocalBounds().toFloat());

    // Draw the spectrogram image, aligning it correctly to its bounds
    g.drawImage(frequencySpectrumImage, 0, 0, frequencySpectrumImage.getWidth(), frequencySpectrumImage.getHeight(),
        0, 0, frequencySpectrumImage.getWidth(), frequencySpectrumImage.getHeight());
}

/*======================================================================================*/
void FrequencySpectrum::resized() 
/*======================================================================================*/
{

}