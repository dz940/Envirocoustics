#pragma once
#include <JuceHeader.h>
#include "WaveformDisplay.h" 

class AudioReader : public juce::Component,
    public juce::ChangeListener
{
public:
    AudioReader();
    ~AudioReader() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void setWaveformDisplay(WaveformDisplay* display) { waveformDisplay = display; }
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);


private:
    void loadAudioFile();
    void loadFile(const juce::File& file);
    void playAudio();
    void stopAudio();

    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    juce::AudioFormatManager formatManager;
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    juce::AudioTransportSource transportSource;

    std::unique_ptr<juce::AudioFormatReaderSource> audioReader;
    std::unique_ptr<juce::FileChooser> chooser;
    WaveformDisplay* waveformDisplay = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioReader)
};
