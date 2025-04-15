#include "FrequencySpectrum.h"
#include "MainComponent.h"

/*======================================================================================*/
FrequencySpectrum::FrequencySpectrum(int nWidth, int nHeight)
    : m_iFrequencySpectrumImage(Image::ARGB, nWidth, nHeight, true)
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
void FrequencySpectrum::vPushBuffer(const AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    auto* pChannelData = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        fifo.getWritePointer(0)[m_nFifoIndex++] = pChannelData[i];

        if (m_nFifoIndex >= fftSize)
        {
            m_nFifoIndex = 0;
            vDrawNextFrameOfFrequencySpectrum();
        }
    }
}

/*======================================================================================*/
void FrequencySpectrum::vApplyDecay()
/*======================================================================================*/
{
    float fDecayFactor = 0.1f;
    Graphics g(m_iFrequencySpectrumImage);
    g.setColour(Colours::black.withAlpha(1.0f - fDecayFactor));
    g.fillRect(m_iFrequencySpectrumImage.getBounds()); // Fades out the old image
}

/*======================================================================================*/
void FrequencySpectrum::vDrawNextFrameOfFrequencySpectrum()
/*======================================================================================*/
{
    const MessageManagerLock mmLock;
    if (!mmLock.lockWasGained()) return;

    HeapBlock<float> fftData(fftSize * 2, true);
    FloatVectorOperations::copy(fftData, fifo.getReadPointer(0), fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);
    
    vApplyDecay();

    int nWidth = m_iFrequencySpectrumImage.getWidth();
    int nHeight = m_iFrequencySpectrumImage.getHeight();

    Path frequencyCurve;

    float fMinFreq = 20.0f;
    float fMaxFreq = 20000.0f;
    float fLogMin = std::log10(fMinFreq);
    float fLogMax = std::log10(fMaxFreq);

    float fPrevX = 0, fPrevY = (float)nHeight;

    frequencyCurve.startNewSubPath(0, (float)nHeight); // **Anchor the path to bottom-left**

    for (int x = 0; x < nWidth; ++x)
    {
        float fNormX = (float)x / nWidth;
        float fLogFreq = fLogMin + fNormX * (fLogMax - fLogMin);
        float fFreq = std::pow(10.0f, fLogFreq);

        // Convert frequency to bin index
        float binFreqResolution = (float)48000 / (float)fftSize;
        float fBinPos = fFreq / binFreqResolution;

        int nBinIndex = jlimit(0, (fftSize / 2) - 1, (int)fBinPos);
        int nNextBinIndex = jlimit(0, (fftSize / 2) - 1, nBinIndex + 1);
        float fBinFraction = fBinPos - nBinIndex;

        float fMagnitude1 = fftData[nBinIndex];
        float fMagnitude2 = fftData[nNextBinIndex];

        float fMagnitude = fMagnitude1 * (1.0f - fBinFraction) + fMagnitude2 * fBinFraction;
        fMagnitude = Decibels::gainToDecibels(fMagnitude + 1e-6f);
        fMagnitude = jmap(fMagnitude, 0.0f, 50.0f, 0.0f, 1.0f);
        fMagnitude = jlimit(0.0f, 1.0f, fMagnitude);

        int yPos = (int)(nHeight - 1 - (fMagnitude * nHeight));

        if (x == 0)
            frequencyCurve.lineTo((float)x, (float)yPos); // Start connecting from (0, yPos)
        else
            frequencyCurve.quadraticTo(fPrevX, fPrevY, (float)x, (float)yPos); // Smooth transition

        fPrevX = (float)x;
        fPrevY = (float)yPos;
    }

    frequencyCurve.lineTo((float)nWidth, (float)nHeight); // **Close at bottom-right**
    frequencyCurve.lineTo(0, (float)nHeight);     // **Return to bottom-left**
    frequencyCurve.closeSubPath();        // **Ensure a fully enclosed fill**

    // **Apply a Vertical Gradient**
    ColourGradient gradient(
        Colours::cyan.withAlpha(0.9f), (float)(nWidth / 2), 0,
        Colours::blue.withAlpha(0.3f), (float)(nWidth / 2), (float)nHeight,
        false
    );

    Graphics g(m_iFrequencySpectrumImage);
    g.setGradientFill(gradient);
    g.fillPath(frequencyCurve); // **Now properly enclosed and filled!**

    // Frequency labels & grid lines (logarithmic spacing)
    std::vector<double> dMajorFreqs = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
    float fLogMinFreq = std::log10(20.0f);
    float fLogMaxFreq = std::log10(20000.0f);
    for (double freq : dMajorFreqs)
    {
        float x = jmap((float)std::log10(freq), fLogMinFreq, fLogMaxFreq, 0.0f, (float)nWidth);

        // Draw vertical grid line
        g.setColour(Colours::grey.withAlpha(0.3f));
        g.drawLine(x, 0, x, (float)nHeight);
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
    g.drawImage(m_iFrequencySpectrumImage, 0, 0, m_iFrequencySpectrumImage.getWidth(), m_iFrequencySpectrumImage.getHeight(),
        0, 0, m_iFrequencySpectrumImage.getWidth(), m_iFrequencySpectrumImage.getHeight());
}