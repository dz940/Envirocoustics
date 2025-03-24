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

#define WIND_DIRECTION_UPWIND       0
#define WIND_DIRECTION_DOWNWIND     1

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
class ResponseComponent;

class MainComponent : public AudioAppComponent,
    public ChangeListener
{
public:
    MainComponent();
    ~MainComponent() override;

    void vSwitchToSpectrogram();
    void vSwitchToSpectrum();
    void vSetParameter(const int nParameterType, const int nValue, const bool bUpdateDisplay);
    void vUpdateConditionControls();
    int nGetParameter(const int nParameterType);
    double dCalculateWindLoss(const double dFrequency, const double dDistance, const double dWindSpeed, const bool bWindDirection);
    double dCalculateTemperatureGradientLoss(const double dFrequency, const double dDistance, const int nTemperatureGradient);
    double dCalculateAirAttenuationPerMetre(const double dFreq, const double dTemperature, const double dRelativeHumidity, const double dAtmosphericPressure);
    
    // Overriden functions
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;

private:

    typedef enum
    {
        Stopped,
        Playing,
        Starting,
        Pausing,
        Stopping
    }transportSourceState_t;

    AudioDeviceManager m_cAudioDeviceManager;
    transportSourceState_t m_cTransportSourceState;

    void vOpenTextButtonClicked();
    void vPlayAudioButtonClicked();
    void vStopAudioButtonClicked();
    void vPauseAudioButtonClicked();
    void vTransportSourceStateChanged(transportSourceState_t state);
    void changeListenerCallback(ChangeBroadcaster* source) override;

    void vApplyDSPProcessing(juce::AudioBuffer<float>& buffer);
    void vUpdateSystemResponse();

    TextButton* m_pcOpenFileButton, *m_pcPlayAudioButton, *m_pcStopAudioButton, *m_pcPauseAudioButton, *m_pcShowSpectrogramButton, *m_pcShowSpectrumButton;

    AudioFormatManager m_cFormatManager;
    std::unique_ptr<AudioFormatReaderSource> m_pcPlaySource;
    AudioTransportSource m_cTransportSource;
    std::unique_ptr<FileChooser> m_pcFileChooser;

    WaveformDisplay* m_pcWaveformDisplay;
    ConditionControls* m_pcConditionControls;
    WeatherPresets* m_pcWeatherPresets;
    DistanceGraphic* m_pcDistanceGraphic;
    VolumeControl* m_pcVolumeControl;
    SpectrogramComponent* m_pcPreProcessingSpectrogram, *m_pcPostProcessingSpectrogram;
    SpectrumComponent* m_pcPreProcessingFrequencyAnalyser, *m_pcPostProcessingFrequencyAnalyser;
    ResponseComponent* m_pcResponseCurve;

    Image m_iTitleImage;

    int m_nDistance = 100;
    int m_nTemperature = 20;
    int m_nHumidity = 50;
    int m_nWindSpeed = 10;
    int m_nPressure = 950;
    int m_nWindDirection = WIND_DIRECTION_UPWIND;
    bool m_bPrecipitation = OFF;
    int m_nTempGradient = TEMPERATURE_LAPSE;
    bool m_bCloudCover = OFF;

    std::vector<double> m_dTargetMagnitudeResponse;

    juce::TooltipWindow m_wTooltipWindow{ this }; // Enables tooltips in this component

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};