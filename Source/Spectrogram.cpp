#include "Spectrogram.h"
#include "MainComponent.h"

/*======================================================================================*/
Spectrogram::Spectrogram(int nWidth, int nHeight)
    : m_iSpectrogramImage(Image::RGB, nWidth, nHeight, true)
/*======================================================================================*/
{
    fifo.setSize(1, fftSize); // One-channel buffer
    Colour clBackground = Colour::fromHSV(0.67f, 1.0f, 1.0f, 1.0f);
    m_iSpectrogramImage.clear(m_iSpectrogramImage.getBounds(), clBackground);
}

/*======================================================================================*/
Spectrogram::~Spectrogram() 
/*======================================================================================*/
{
}

/*======================================================================================*/
void Spectrogram::vPushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    auto* channelData = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        fifo.getWritePointer(0)[m_nFifoIndex++] = channelData[i];
        if (m_nFifoIndex >= fftSize)
        {
            m_nFifoIndex = 0;
            vDrawNextFrameOfSpectrogram();
        }
    }
}

/*======================================================================================*/
void Spectrogram::vDrawNextFrameOfSpectrogram()
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
    int spectrogramWidth = m_iSpectrogramImage.getWidth();
    vShiftImageLeft(); // Scroll image left for real-time effect

    for (int y = 0; y < m_iSpectrogramImage.getHeight(); ++y)
    {
        int nFftIndex = int((fftSize / 2 - 1) * (1.0f - (float)y / m_iSpectrogramImage.getHeight())); // Flip index
        nFftIndex = jlimit(0, (fftSize / 2) - 1, nFftIndex);

        float fMagnitude = fftData[nFftIndex] * 20.0f; // Adjust scale
        float fHue = jlimit(0.0f, 0.67f, 0.67f - (fMagnitude * 0.005f)); // Map high magnitude to red, low to blue
        Colour clColor = Colour::fromHSV(fHue, 1.0f, 1.0f, 1.0f);
        m_iSpectrogramImage.setPixelAt(spectrogramWidth - 1, y, clColor);
    }
    repaint();
}

/*======================================================================================*/
void Spectrogram::vShiftImageLeft()
/*======================================================================================*/
{
    //auto area = spectrogramImage.getBounds().withTrimmedLeft(1);
    Rectangle<int> rcArea = getLocalBounds().withTrimmedLeft(1);
    m_iSpectrogramImage.moveImageSection(0, 0, 1, 0, rcArea.getWidth(), rcArea.getHeight());
}

/*======================================================================================*/
void Spectrogram::paint(Graphics& g)
/*======================================================================================*/
{
    // Get the position and size of the Spectrogram component
    Rectangle<float> rcImage(getLocalBounds().toFloat());

    // Draw the spectrogram image, aligning it correctly to its bounds
    g.drawImage(m_iSpectrogramImage, 0, 0, m_iSpectrogramImage.getWidth(), m_iSpectrogramImage.getHeight(),
        0, 0, m_iSpectrogramImage.getWidth(), m_iSpectrogramImage.getHeight());
}
