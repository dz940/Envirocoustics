#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class FrequencySpectrum : public Component
{
public:
    FrequencySpectrum(int nWidth, int nHeight);
    ~FrequencySpectrum() override;
    void paint(Graphics& g) override;
    void vPushBuffer(const AudioBuffer<float>& buffer);

private:
    void vDrawNextFrameOfFrequencySpectrum();
    void vApplyDecay();

    static constexpr int fftSize = 1024; // FFT size (higher = better resolution)
    static constexpr int fftOrder = 10;  // log2(fftSize) for FFT calculations

    dsp::FFT forwardFFT{ fftOrder };
    Image m_iFrequencySpectrumImage; 
    AudioBuffer<float> fifo;
    int m_nFifoIndex = 0;
};
