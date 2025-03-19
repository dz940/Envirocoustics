#include "Spectrogram.h"
#include "MainComponent.h"

/*======================================================================================*/
Spectrogram::Spectrogram(int nWidth, int nHeight)
    : spectrogramImage(Image::RGB, nWidth, nHeight, true)
/*======================================================================================*/
{
    fifo.setSize(1, fftSize); // One-channel buffer
    Colour clBackground = Colour::fromHSV(0.67f, 1.0f, 1.0f, 1.0f);
    spectrogramImage.clear(spectrogramImage.getBounds(), clBackground);
}

/*======================================================================================*/
Spectrogram::~Spectrogram() 
/*======================================================================================*/
{
}

/*======================================================================================*/
void Spectrogram::pushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    auto* channelData = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        fifo.getWritePointer(0)[fifoIndex++] = channelData[i];
        if (fifoIndex >= fftSize)
        {
            fifoIndex = 0;
            drawNextFrameOfSpectrogram();
        }
    }
}

/*======================================================================================*/
void Spectrogram::drawNextFrameOfSpectrogram()
/*======================================================================================*/
{
    // Ensure thread safety when modifying UI components
    const MessageManagerLock mmLock;
    if (!mmLock.lockWasGained()) return; // If lock fails, exit safely

    HeapBlock<float> fftData(fftSize * 2, true); // Zero-initialized FFT buffer
    FloatVectorOperations::copy(fftData, fifo.getReadPointer(0), fftSize);

    HeapBlock<float> window(fftSize);
    dsp::WindowingFunction<float>::fillWindowingTables(window.getData(), fftSize, dsp::WindowingFunction<float>::hann, false);
    FloatVectorOperations::multiply(fftData, window.getData(), fftSize);

    forwardFFT.performFrequencyOnlyForwardTransform(fftData);
    int spectrogramWidth = spectrogramImage.getWidth();
    shiftImageLeft(); // Scroll image left for real-time effect

    for (int y = 0; y < spectrogramImage.getHeight(); ++y)
    {
        int fftIndex = int((fftSize / 2 - 1) * (1.0f - (float)y / spectrogramImage.getHeight())); // Flip index
        fftIndex = jlimit(0, (fftSize / 2) - 1, fftIndex);

        float magnitude = fftData[fftIndex] * 20.0f; // Adjust scale
        float hue = jlimit(0.0f, 0.67f, 0.67f - (magnitude * 0.005f)); // Map high magnitude to red, low to blue
        Colour color = Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f);
        spectrogramImage.setPixelAt(spectrogramWidth - 1, y, color);
    }
    repaint();
}

/*======================================================================================*/
void Spectrogram::shiftImageLeft()
/*======================================================================================*/
{
    //auto area = spectrogramImage.getBounds().withTrimmedLeft(1);
    auto area = getLocalBounds().withTrimmedLeft(1);
    spectrogramImage.moveImageSection(0, 0, 1, 0, area.getWidth(), area.getHeight());
}

/*======================================================================================*/
void Spectrogram::paint(Graphics& g)
/*======================================================================================*/
{
    // Get the position and size of the Spectrogram component
    Rectangle<float> rcImage(getLocalBounds().toFloat());

    // Draw the spectrogram image, aligning it correctly to its bounds
    g.drawImage(spectrogramImage, 0, 0, spectrogramImage.getWidth(), spectrogramImage.getHeight(),
        0, 0, spectrogramImage.getWidth(), spectrogramImage.getHeight());
}
