#pragma once

#include <JuceHeader.h>
using namespace juce;

// Presets
#define SUNNY_PRESET    1
#define WINDY_PRESET    2
#define RAINY_PRESET    3
#define SNOWY_PRESET    4

// Parameters
#define PARAMETER_TEMPERATURE       1
#define PARAMETER_WIND_SPEED        2
#define PARAMETER_HUMIDITY          3
#define PARAMETER_PRESSURE          4
#define PARAMETER_WIND_DIRECTION    5
#define PARAMETER_PRECIPITATION     6
#define PARAMETER_TEMP_GRADIENT     7
#define PARAMETER_CLOUD_COVER       8
#define PARAMETER_DISTANCE          9
#define PARAMETER_MAKEUP_GAIN       10

// Wind direction
#define WIND_DIRECTION_UPWIND       0
#define WIND_DIRECTION_DOWNWIND     1

// Temperature gradient
#define TEMPERATURE_LAPSE       0
#define TEMPERATURE_INVERSION   1

// Colours
#define COLOUR_COMPONENT_BACKGROUND     0xFF43C59E  // Pale green
#define COLOUR_COMPONENT_OUTLINE        0xFF3D7068  // Dark grey

// States
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
    void vSetParameter(const int nParameterType, const double nValue, const bool bUpdateDisplay);
    void vUpdateWindFilterCoeffs();
    void vUpdateTempFilterCoeffs();
    void vUpdateMainFilterCoeffs();
    void vUpdateConditionControls();
    double dGetParameter(const int nParameterType);
    double dCalculateAirAttenuationPerMetre(const double dFreq, const double dTemperature, const double dRelativeHumidity, const double dAtmosphericPressure);
    
    // Overriden functions
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void vShowVolumeWarningBox();
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

    void vApplyDSPProcessing(AudioBuffer<float>& buffer);
    void vUpdateSystemResponse();

    TextButton* m_pcOpenFileButton, *m_pcPlayAudioButton, *m_pcStopAudioButton, *m_pcPauseAudioButton, *m_pcShowSpectrogramButton, *m_pcShowSpectrumButton;
    ToggleButton* m_pcDisablePowerLossCheck;

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

    double m_dDistance = 100.0;
    double m_dTemperature = 20;
    double m_dHumidity = 50;
    double m_dWindSpeed = 10;
    double m_dPressure = 950;
    bool m_bWindDirection = WIND_DIRECTION_UPWIND;
    bool m_bPrecipitation = OFF;
    bool m_bTempGradient = TEMPERATURE_LAPSE;
    bool m_bCloudCover = OFF;

    bool m_bEnableMakeupGain = true;

    std::vector<double> m_dTargetMagnitudeResponse;
    CriticalSection filterLock;

    IIRFilter m_cMainFilterL, m_cMainFilterR;
    IIRFilter m_cWindFilterL, m_cWindFilterR;
    IIRFilter m_cTempFilterL, m_cTempFilterR;

    IIRCoefficients m_cMainIIRCoeffs;
    IIRCoefficients m_cWindIIRCoeffs;
    IIRCoefficients m_cTempIIRCoeffs;

    std::unique_ptr<AlertWindow> m_pAlertWindow;

    TooltipWindow m_wTooltipWindow{ this }; // Enables tooltips in this component

    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};