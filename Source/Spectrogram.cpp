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
    int nSpectrogramWidth = m_iSpectrogramImage.getWidth();
    vShiftImageLeft(); // Scroll image left for real-time effect

    float fMinFreq = 20.0f;        // Lowest frequency (20 Hz)
    float fMaxFreq = 15000.0f;     // Highest frequency (15,000 Hz)
    float fNyquist = 48000 / 2.0f; // Nyquist frequency (half of the sample rate)

    for (int y = 0; y < m_iSpectrogramImage.getHeight(); ++y)
    {
        // Reverse the Y-axis and map to a frequency range between 20 Hz and 15,000 Hz
        float fFreq = fMaxFreq - (fMaxFreq - fMinFreq) * (float)y / (m_iSpectrogramImage.getHeight() - 1);

        // Convert that frequency to the corresponding FFT bin index
        int fFftIndex = int(fFreq / fNyquist * (fftSize / 2));

        fFftIndex = jlimit(0, (fftSize / 2) - 1, fFftIndex); // Ensure within bounds

        float fMagnitude = fftData[fFftIndex];
        float fMagnitudeDB = 20.0f * std::log10(jmax(fMagnitude, 1.0e-9f)); // dB conversion

        float fMinDb = 0.0f;
        float fMaxDb = 50.0f;
        float fNormalisedMag = jlimit(0.0f, 1.0f, (fMagnitudeDB - fMinDb) / (fMaxDb - fMinDb));

        float hue = 0.67f - fNormalisedMag * 0.67f;
        Colour clColor = Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f);

        // Set the color at the current Y pixel
        m_iSpectrogramImage.setPixelAt(nSpectrogramWidth - 1, y, clColor);
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
