#include "AudioReader.h"

/*======================================================================================*/
AudioReader::AudioReader()
/*======================================================================================*/
{
    // Configure the file browser button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load Audio File");
    loadButton.onClick = [this]() { loadAudioFile(); };

    // Configure the play button
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this]() { playAudio(); };

    // Configure the stop button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this]() { stopAudio(); };

    // Register supported audio formats
    formatManager.registerBasicFormats();

    // Configure the audio device manager
    deviceManager.initialiseWithDefaultDevices(0, 2); // 0 inputs, 2 outputs
    deviceManager.addAudioCallback(&audioSourcePlayer);
    audioSourcePlayer.setSource(&transportSource);

    // Add the transport source as a change listener
    transportSource.addChangeListener(this);

    setSize(800, 600);
}

AudioReader::~AudioReader()
{
    transportSource.removeChangeListener(this);
    transportSource.setSource(nullptr);
    audioSourcePlayer.setSource(nullptr);
    deviceManager.removeAudioCallback(&audioSourcePlayer);
}

void AudioReader::resized()
{
    auto area = getLocalBounds().reduced(10);
    loadButton.setBounds(area.removeFromTop(40));
    playButton.setBounds(area.removeFromTop(40));
    stopButton.setBounds(area.removeFromTop(40));
}

void AudioReader::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AudioReader::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource && transportSource.hasStreamFinished())
    {
        DBG("Audio playback finished.");
        transportSource.setPosition(0);
    }
}

void AudioReader::loadAudioFile()
{
    chooser = std::make_unique<juce::FileChooser>(
        "Please select the mp3 you want to load...",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.mp3");

    auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File mp3File(chooser.getResult());
            loadFile(mp3File);
        });
}

void AudioReader::loadFile(const juce::File& file)
{
    transportSource.stop();
    transportSource.setSource(nullptr);
    audioReader.reset();

    if (auto* reader = formatManager.createReaderFor(file))
    {
        DBG("Loaded file: " + file.getFullPathName());
        audioReader.reset(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(audioReader.get(), 0, nullptr, reader->sampleRate);
    }
    else
    {
        DBG("Failed to load file: " + file.getFullPathName());
    }
}

void AudioReader::playAudio()
{
    if (transportSource.getLengthInSeconds() > 0)
    {
        DBG("Playing audio...");
        transportSource.start();
    }
    else
    {
        DBG("No audio loaded to play.");
    }
}

void AudioReader::stopAudio()
{
    transportSource.stop();
}

void AudioReader::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{


    bufferToFill.clearActiveBufferRegion(); // Make sure to clear the buffer
}