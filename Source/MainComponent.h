#pragma once

#include <JuceHeader.h>
using namespace juce;

#define SUNNY_PRESET    1
#define WINDY_PRESET    2
#define RAINY_PRESET    3
#define SNOWY_PRESET    4

#define PARAMETER_TEMPERATURE       1
#define PARAMETER_WIND_SPEED        2
#define PARAMETER_HUMIDITY          3
#define PARAMETER_PRESSURE          4
#define PARAMETER_WIND_DIRECTION    5
#define PARAMETER_PRECIPITATION     6
#define PARAMETER_TEMP_GRADIENT     7
#define PARAMETER_CLOUD_COVER       8
#define PARAMETER_DISTANCE          9

#define WIND_DIRECTION_LEFT     0
#define WIND_DIRECTION_RIGHT    1

#define TEMPERATURE_LAPSE       0
#define TEMPERATURE_INVERSION   1

#define OFF  0
#define ON   1

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class DistanceGraphic;
class WeatherPresets;
class WaveformDisplay;
class ConditionControls;
class VolumeControl;
class SpectrogramComponent;
class SpectrumComponent;
class ResponseCurve;

class MainComponent : public AudioAppComponent,
    public ChangeListener
{
public:
    MainComponent();
    ~MainComponent() override;

    void switchToSpectrogram();
    void switchToSpectrum();
    void vSetParameter(int nParameterType, int nValue, bool bUpdateDisplay);
    void vUpdateConditionControls();
    int nGetParameter(int nParameterType);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    double dCalculateAirAttenuationPerMetre(const double dFreq, const double dTemperature, const double dRelativeHumidity, const double dAtmosphericPressure);
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;

private:

    AudioDeviceManager customDeviceManager;

    typedef enum
    {
        Stopped,
        Playing,
        Starting,
        Pausing,
        Stopping
    }transportSourceState_t;

    transportSourceState_t transportSourceState;

    void openTextButtonClicked();
    void playAudioButtonClicked();
    void stopAudioButtonClicked();
    void pauseAudioButtonClicked();
    void transportSourceStateChanged(transportSourceState_t state);
    void thumbnailChanged();
    void changeListenerCallback(ChangeBroadcaster* source) override;

    void DSPEngine(juce::AudioBuffer<float>& buffer);

    void updateFilter(const int nDistanceVal);

    TextButton openFileButton, playAudioButton, stopAudioButton, pauseAudioButton, spectrogramButton, spectrumButton;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> playSource;
    AudioTransportSource transportSource;
    std::unique_ptr<FileChooser> chooser;

    WaveformDisplay* waveformDisplay;
    ConditionControls* conditionControls;
    WeatherPresets* weatherPresets;
    DistanceGraphic* distanceGraphic;
    VolumeControl* volumeControl;
    SpectrogramComponent* spectrogram1, *spectrogram2;
    SpectrumComponent* frequencyAnalyser1, *frequencyAnalyser2;
    ResponseCurve* responseCurve;

    Image titleImage;

    int nDistance = 100;
    int nTemperature = 20;
    int nHumidity = 50;
    int nWindSpeed = 10;
    int nPressure = 950;
    int nWindDirection = WIND_DIRECTION_LEFT;
    bool bPrecipitation = OFF;
    int nTempGradient = TEMPERATURE_LAPSE;
    bool bCloudCover = OFF;

    juce::dsp::IIR::Filter<float> lowPassFilter;

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};