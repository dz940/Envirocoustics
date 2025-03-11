#pragma once
#include <JuceHeader.h>

class MainComponent;
using namespace juce;

class FrequencySpectrum : public Component
{
public:
    FrequencySpectrum(int nWidth, int nHeight);
    ~FrequencySpectrum() override;
    void pushBuffer(const AudioBuffer<float>& buffer);
    void paint(Graphics& g) override;
    void resized() override;
    void smoothFFTData(float* fftData, int size);
    float interpolate(float v1, float v2, float t);
    Colour blendColors(Colour c1, Colour c2, float alpha);

private:
    void drawNextFrameOfFrequencySpectrum();
    void applyDecay();

    static constexpr int fftSize = 1024; // FFT size (higher = better resolution)
    static constexpr int fftOrder = 10;  // log2(fftSize) for FFT calculations

    dsp::FFT forwardFFT{ fftOrder };
    Image frequencySpectrumImage; 
    AudioBuffer<float> fifo;
    int fifoIndex = 0;
};
