#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class Spectrogram : public Component
{
public:
    Spectrogram(int nWidth, int nHeight);
    ~Spectrogram() override;

    void vPushBuffer(const AudioBuffer<float>& buffer);
    void paint(Graphics& g) override;

private:

    static constexpr int fftSize = 1024; // FFT size (higher = better resolution)
    static constexpr int fftOrder = 10;  // log2(fftSize) for FFT calculations

    dsp::FFT forwardFFT{ fftOrder };
    Image m_iSpectrogramImage; // Image buffer for visualization
    AudioBuffer<float> fifo; // Circular buffer for incoming audio
    int m_nFifoIndex = 0;

    void vDrawNextFrameOfSpectrogram();
    void vShiftImageLeft();
};
