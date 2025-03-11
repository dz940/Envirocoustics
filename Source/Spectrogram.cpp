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

//void Spectrogram::drawNextFrameOfSpectrogram()
//{
//    // Ensure thread safety when modifying UI components
//    const MessageManagerLock mmLock;
//    if (!mmLock.lockWasGained()) return; // If lock fails, exit safely
//
//    HeapBlock<float> fftData(fftSize * 2, true); // Zero-initialized FFT buffer
//    FloatVectorOperations::copy(fftData, fifo.getReadPointer(0), fftSize);
//    forwardFFT.performFrequencyOnlyForwardTransform(fftData); // Perform the FFT
//
//    int spectrogramWidth = spectrogramImage.getWidth();
//    shiftImageLeft(); // Scroll image left for real-time effect
//
//    int sampleRate = 44100;
//    // Logarithmic scaling for frequency axis (mapping pixel to frequency)
//    const float minFreq = 20.0f; // Minimum frequency (e.g., 20 Hz for human hearing)
//    const float maxFreq = sampleRate / 2.0f; // Maximum frequency is Nyquist (half the sample rate)
//
//    // Loop through each row (y-axis), which corresponds to a frequency band
//    for (int y = 0; y < spectrogramImage.getHeight(); ++y)
//    {
//        // Logarithmic frequency scaling: map y position to a logarithmic frequency
//        // Invert the y-axis to ensure low frequencies are at the bottom (reverse the calculation)
//        float logFreq = minFreq * pow(10.0f, float(spectrogramImage.getHeight() - y - 1) / spectrogramImage.getHeight() * log10(maxFreq / minFreq));
//
//        // Map this logarithmic frequency to the corresponding FFT index
//        int fftIndex = int(logFreq / (sampleRate / fftSize));  // Convert logFreq to FFT bin index
//        fftIndex = jlimit(0, (fftSize / 2) - 1, fftIndex);  // Ensure within bounds of FFT bins
//
//        // Compute the magnitude of the frequency at this bin and scale it for visual representation
//        float magnitude = fftData[fftIndex] * 20.0f;  // Adjust scale for better visibility
//
//        // Map the magnitude to a color hue: higher magnitudes -> red, lower magnitudes -> blue
//        float hue = jlimit(0.0f, 0.67f, 0.67f - (magnitude * 0.005f));
//        Colour color = Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f);  // Create color for pixel
//
//        // Set the pixel color at the corresponding y position for the current frequency bin
//        spectrogramImage.setPixelAt(spectrogramWidth - 1, y, color);
//    }
//
//    repaint(); // Safe repaint since we have the lock
//}

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
