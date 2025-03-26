#include "MainComponent.h"
#include "WeatherPresets.h"
#include "DistanceGraphic.h"
#include "WaveformDisplay.h"
#include "ConditionControls.h"
#include "SpectrumComponent.h"
#include "VolumeControl.h"
#include "SpectrogramComponent.h"
#include "ResponseComponent.h"
#include "AtmosphericAbsorption.h"
#include <atlstr.h>

/*======================================================================================*/
MainComponent::MainComponent() : AudioAppComponent(m_cAudioDeviceManager)
/*======================================================================================*/
{
    // Load logo
    m_iTitleImage = ImageFileFormat::loadFrom(BinaryData::title_png, BinaryData::title_pngSize);
    setOpaque(true);

    // Creating all components
    m_pcDistanceGraphic = new DistanceGraphic(*this);
    addAndMakeVisible(m_pcDistanceGraphic);
    m_pcDistanceGraphic->setOpaque(true);

    m_pcWeatherPresets = new WeatherPresets(*this);
    addAndMakeVisible(m_pcWeatherPresets);
    m_pcWeatherPresets->setOpaque(true);

    m_pcWaveformDisplay = new WaveformDisplay(*this);
    addAndMakeVisible(m_pcWaveformDisplay);
    m_pcWaveformDisplay->setOpaque(true);

    m_pcConditionControls = new ConditionControls(*this);
    addAndMakeVisible(m_pcConditionControls);
    m_pcConditionControls->setOpaque(true);

    m_pcVolumeControl = new VolumeControl(*this);
    addAndMakeVisible(m_pcVolumeControl);
    m_pcVolumeControl->setOpaque(true);

    m_pcPreProcessingSpectrogram = new SpectrogramComponent(*this);
    addAndMakeVisible(m_pcPreProcessingSpectrogram);
    m_pcPreProcessingSpectrogram->setOpaque(true);
    m_pcPreProcessingSpectrogram->setVisible(false);

    m_pcPostProcessingSpectrogram = new SpectrogramComponent(*this);
    addAndMakeVisible(m_pcPostProcessingSpectrogram);
    m_pcPostProcessingSpectrogram->setOpaque(true);
    m_pcPostProcessingSpectrogram->setVisible(false);

    m_pcPreProcessingFrequencyAnalyser = new SpectrumComponent(*this);
    addAndMakeVisible(m_pcPreProcessingFrequencyAnalyser);
    m_pcPreProcessingFrequencyAnalyser->setOpaque(true);

    m_pcPostProcessingFrequencyAnalyser = new SpectrumComponent(*this);
    addAndMakeVisible(m_pcPostProcessingFrequencyAnalyser);
    m_pcPostProcessingFrequencyAnalyser->setOpaque(true);

    m_pcResponseCurve = new ResponseComponent(*this);
    addAndMakeVisible(m_pcResponseCurve);
    m_pcResponseCurve->setOpaque(true);

    // Initialising the transport source state
    m_cTransportSourceState = Stopped;
    m_cAudioDeviceManager.initialise(0, 2, nullptr, true);
    setAudioChannels(0, 2); // Stereo audio

    // Setting up the main component buttons
    m_pcOpenFileButton = new TextButton();
    m_pcOpenFileButton->setButtonText("Open File");
    m_pcOpenFileButton->onClick = [this] {vOpenTextButtonClicked(); };
    m_pcOpenFileButton->setColour(TextButton::buttonColourId, Colours::darkseagreen);
    addAndMakeVisible(m_pcOpenFileButton);

    m_pcPlayAudioButton = new TextButton();
    m_pcPlayAudioButton->setButtonText("Play");
    m_pcPlayAudioButton->onClick = [this] {vPlayAudioButtonClicked(); };
    m_pcPlayAudioButton->setColour(TextButton::buttonColourId, Colours::darkseagreen);
    m_pcPlayAudioButton->setEnabled(false);
    addAndMakeVisible(m_pcPlayAudioButton);

    m_pcPauseAudioButton = new TextButton();
    m_pcPauseAudioButton->setButtonText("Pause");
    m_pcPauseAudioButton->onClick = [this] {vPauseAudioButtonClicked(); };
    m_pcPauseAudioButton->setColour(TextButton::buttonColourId, Colours::darkseagreen);
    m_pcPauseAudioButton->setEnabled(false);
    addAndMakeVisible(m_pcPauseAudioButton);

    m_pcStopAudioButton = new TextButton();
    m_pcStopAudioButton->setButtonText("Stop");
    m_pcStopAudioButton->onClick = [this] {vStopAudioButtonClicked(); };
    m_pcStopAudioButton->setColour(TextButton::buttonColourId, Colours::darkseagreen);
    m_pcStopAudioButton->setEnabled(false);
    addAndMakeVisible(m_pcStopAudioButton);

    m_pcShowSpectrumButton = new TextButton();
    m_pcShowSpectrumButton->setButtonText("Spectrum");
    m_pcShowSpectrumButton->onClick = [this] {vSwitchToSpectrum(); };
    m_pcShowSpectrumButton->setColour(TextButton::buttonOnColourId, Colours::green);
    m_pcShowSpectrumButton->setEnabled(true);
    m_pcShowSpectrumButton->setClickingTogglesState(true);
    m_pcShowSpectrumButton->setRadioGroupId(1);
    m_pcShowSpectrumButton->setToggleState(true, dontSendNotification);
    addAndMakeVisible(m_pcShowSpectrumButton);

    m_pcShowSpectrogramButton = new TextButton();
    m_pcShowSpectrogramButton->setButtonText("Spectrogram");
    m_pcShowSpectrogramButton->onClick = [this] {vSwitchToSpectrogram(); };
    m_pcShowSpectrogramButton->setColour(TextButton::buttonOnColourId, Colours::green);
    m_pcShowSpectrogramButton->setEnabled(true);
    m_pcShowSpectrogramButton->setClickingTogglesState(true);
    m_pcShowSpectrogramButton->setRadioGroupId(1);
    addAndMakeVisible(m_pcShowSpectrogramButton);

    m_cFormatManager.registerBasicFormats();
    m_cTransportSource.addChangeListener(this);

    mainFilterL.reset();
    mainFilterR.reset();

    m_wTooltipWindow.setMillisecondsBeforeTipAppears(500);
    // Set the size of the component after adding child components.
    setSize(960, 860);
}

/*======================================================================================*/
MainComponent::~MainComponent()
/*======================================================================================*/
{
    // Shut down the audio device and clear the audio source.
    shutdownAudio();

    // Delete child components
    delete m_pcDistanceGraphic;
    delete m_pcWeatherPresets;
    delete m_pcWaveformDisplay;
    delete m_pcConditionControls;
    delete m_pcVolumeControl;
    delete m_pcResponseCurve;
    delete m_pcPreProcessingSpectrogram;
    delete m_pcPostProcessingSpectrogram;
    delete m_pcPreProcessingFrequencyAnalyser;
    delete m_pcPostProcessingFrequencyAnalyser;
    delete m_pcOpenFileButton;
    delete m_pcPlayAudioButton;
    delete m_pcStopAudioButton;
    delete m_pcPauseAudioButton;
    delete m_pcShowSpectrogramButton;
    delete m_pcShowSpectrumButton;
}

/*======================================================================================*/
void MainComponent::vSwitchToSpectrogram()
/*======================================================================================*/
{
    // Hide frequency spectrum and show spectrogram
    m_pcPreProcessingFrequencyAnalyser->setVisible(false);
    m_pcPostProcessingFrequencyAnalyser->setVisible(false);
    m_pcPreProcessingSpectrogram->setVisible(true);
    m_pcPostProcessingSpectrogram->setVisible(true);
}

/*======================================================================================*/
void MainComponent::vSwitchToSpectrum()
/*======================================================================================*/
{
    // Hide spectrogram and show frequency spectrum
    m_pcPreProcessingSpectrogram->setVisible(false);
    m_pcPostProcessingSpectrogram->setVisible(false);
    m_pcPreProcessingFrequencyAnalyser->setVisible(true);
    m_pcPostProcessingFrequencyAnalyser->setVisible(true);
}

/*======================================================================================*/
void MainComponent::vSetParameter(const int nParameterType, const int nValue, const bool bUpdateDisplay)
/*======================================================================================*/
{
    // Centrally setting paramaters from all the controls
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        {
            m_nTemperature = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_HUMIDITY:
        {
            m_nHumidity = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_WIND_SPEED:
        {
            m_nWindSpeed = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            m_nWindDirection = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_PRECIPITATION:
        {
            m_bPrecipitation = (bool)nValue;

            // Precipitation automatically sets humidity to 90%
            if (m_bPrecipitation)
            { m_nHumidity = 90; }
            else
            { m_nHumidity = 50; }
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            m_nTempGradient = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_PRESSURE:
        {
            m_nPressure = nValue;
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_CLOUD_COVER:
        {
            m_bCloudCover = (bool)nValue;
            break;
        }
        case PARAMETER_DISTANCE:
        {
            m_nDistance = nValue;
            vUpdateSystemResponse();
            break;
        }
    }
    if(bUpdateDisplay)
    { m_pcDistanceGraphic->repaint(); }
}

/*======================================================================================*/
void MainComponent::vUpdateConditionControls()
/*======================================================================================*/
{
    // Complete update and repaint of condition controls
    m_pcConditionControls->vSetParameter(PARAMETER_TEMPERATURE, m_nTemperature);
    m_pcConditionControls->vSetParameter(PARAMETER_WIND_SPEED, m_nWindSpeed);
    m_pcConditionControls->vSetParameter(PARAMETER_HUMIDITY, m_nHumidity);
    m_pcConditionControls->vSetParameter(PARAMETER_TEMP_GRADIENT, m_nTempGradient);
    m_pcConditionControls->vSetParameter(PARAMETER_PRECIPITATION, m_bPrecipitation);
    m_pcConditionControls->vSetParameter(PARAMETER_WIND_DIRECTION, m_nWindDirection);
    m_pcConditionControls->vSetParameter(PARAMETER_PRESSURE, m_nPressure);
    m_pcConditionControls->vSetParameter(PARAMETER_CLOUD_COVER, m_bCloudCover);
    m_pcConditionControls->repaint();
}

/*======================================================================================*/
int MainComponent::nGetParameter(int nParameterType)
/*======================================================================================*/
{
    // Retrieving parameters from central location
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        { return m_nTemperature; }
        case PARAMETER_HUMIDITY:
        { return m_nHumidity; }
        case PARAMETER_WIND_SPEED:
        { return m_nWindSpeed; }
        case PARAMETER_WIND_DIRECTION:
        { return m_nWindDirection; }
        case PARAMETER_PRECIPITATION:
        { return (int)m_bPrecipitation; }
        case PARAMETER_TEMP_GRADIENT:
        {  return m_nTempGradient; }
        case PARAMETER_PRESSURE:
        { return m_nPressure; }
        case PARAMETER_CLOUD_COVER:
        { return m_bCloudCover; }
        case PARAMETER_DISTANCE:
        { return m_nDistance; }
    }
    jassert(false);
    return 0;
}

/*======================================================================================*/
void MainComponent::prepareToPlay(int nSamplesPerBlockExpected, double dSampleRate)
/*======================================================================================*/
{
    m_cTransportSource.prepareToPlay(nSamplesPerBlockExpected, dSampleRate);
}

/*======================================================================================*/
void MainComponent::vOpenTextButtonClicked()
/*======================================================================================*/
{
    // Choose file to load and import into the audio player
    m_pcFileChooser = std::make_unique<FileChooser>(
        "Please select the mp3 you want to load...",
        File::getSpecialLocation(File::userHomeDirectory),
        "*.mp3;*.wav");

    int nFolderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;

    // Launch file browser
    m_pcFileChooser->launchAsync(nFolderChooserFlags, [this](const FileChooser& m_pcFileChooser)
        {
            File cAudioFile(m_pcFileChooser.getResult());
            m_cTransportSource.stop();
            m_cTransportSource.setSource(nullptr);

            if (AudioFormatReader* reader = m_cFormatManager.createReaderFor(cAudioFile))
            {
                DBG("Loaded file: " + cAudioFile.getFullPathName());
                m_pcPlaySource.reset(new AudioFormatReaderSource(reader, true));
                m_cTransportSource.setSource(m_pcPlaySource.get(), 0, nullptr, reader->sampleRate);
                m_cTransportSource.setPosition(0.0);
                m_pcPlayAudioButton->setEnabled(true);
            }
            else
            { DBG("Failed to load file: " + cAudioFile.getFullPathName()); }
        });
}

/*======================================================================================*/
void MainComponent::vPlayAudioButtonClicked()
/*======================================================================================*/
{
    vTransportSourceStateChanged(Starting);
}

/*======================================================================================*/
void MainComponent::vStopAudioButtonClicked()
/*======================================================================================*/
{
    vTransportSourceStateChanged(Stopping);
}

/*======================================================================================*/
void MainComponent::vPauseAudioButtonClicked()
/*======================================================================================*/
{
    vTransportSourceStateChanged(Pausing);
}

/*======================================================================================*/
void MainComponent::vTransportSourceStateChanged(transportSourceState_t state)
/*======================================================================================*/
{
    if (state != m_cTransportSourceState)
    {
        m_cTransportSourceState = state;

        switch (m_cTransportSourceState)
        {
            case Stopped:
            {
                double dPosition = m_cTransportSource.getCurrentPosition();
                double dLength = m_cTransportSource.getLengthInSeconds();
                if (std::abs(dPosition - dLength) < 0.01)
                { m_cTransportSource.setPosition(0.0); }
                break;
            }
            case Playing:
            {
                m_pcPlayAudioButton->setEnabled(false);
                m_pcPauseAudioButton->setEnabled(true);
                m_pcStopAudioButton->setEnabled(true);
                m_pcOpenFileButton->setEnabled(false);
                break;
            }
            case Starting:
            {
                m_pcPlayAudioButton->setEnabled(false);
                m_pcOpenFileButton->setEnabled(false);
                m_pcPauseAudioButton->setEnabled(true);
                m_pcStopAudioButton->setEnabled(true);
                m_cTransportSource.start();
                break;
            }
            case Pausing:
            {
                m_pcPlayAudioButton->setEnabled(true);
                m_pcStopAudioButton->setEnabled(true);
                m_pcOpenFileButton->setEnabled(false);
                m_cTransportSource.stop();
                break;
            }
            case Stopping:
            {
                m_pcPlayAudioButton->setEnabled(true);
                m_pcStopAudioButton->setEnabled(false);
                m_pcPauseAudioButton->setEnabled(false);
                m_pcOpenFileButton->setEnabled(true);
                m_cTransportSource.stop();
                m_cTransportSource.setPosition(0.0);
                break;
            }
        }
    }
}

/*======================================================================================*/
void MainComponent::changeListenerCallback(ChangeBroadcaster* source)
/*======================================================================================*/
{
    if (source == &m_cTransportSource)
    {
        if (m_cTransportSource.isPlaying())
        { vTransportSourceStateChanged(Playing); }
        else
        { vTransportSourceStateChanged(Stopped); }
    }
}

/*======================================================================================*/
void MainComponent::vApplyDSPProcessing(AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    // Initialise the cutoff solver with atmospheric conditions
    double dTempFarenheit = (m_nTemperature * 1.8) + 32; // Atmospheric absorption calculation uses farenheit
    double dPressurePascals = m_nPressure * 100; // Atmoshperic absoption calculation uses pascals
    FilterCutoffSolver cCutoffSolver(m_nHumidity, dTempFarenheit, dPressurePascals);

    double dDistance = (m_nDistance == 0) ? 1 : m_nDistance; // Set minimum distance to 1 so that it doesnt break the calculation
    double dCutoffFrequency = cCutoffSolver.Solve(dDistance); // Calculate the LPF cutoff frequency
    dCutoffFrequency = (dCutoffFrequency > 22000.0) ? 20000.0 : dCutoffFrequency; // Ensure cut off frequency is not greater than 20kHz

    // Set the low-pass filter with the calculated cutoff frequency
    mainFilterL.setCoefficients(IIRCoefficients::makeLowPass(44100, dCutoffFrequency));
    mainFilterR.setCoefficients(IIRCoefficients::makeLowPass(44100, dCutoffFrequency));

    int nNumSamples = buffer.getNumSamples();
    float* pfChannelDataL = buffer.getWritePointer(0);
    float* pfChannelDataR = buffer.getWritePointer(1);

    // Apply the filter to the left and right channels of the audio buffer
    mainFilterL.processSamples(pfChannelDataL, nNumSamples);
    mainFilterR.processSamples(pfChannelDataR, nNumSamples);
    
}

/*======================================================================================*/
void MainComponent::vUpdateSystemResponse()
/*======================================================================================*/
{
    m_pcResponseCurve->repaint();
}

/*======================================================================================*/
double MainComponent::dCalculateWindLoss(const double dFrequency, const double dDistance, const double dWindSpeed, const bool bWindDirection)
/*======================================================================================*/
{
    // Wind attenuation loss
    double dWindLoss = 1.0e-5 * dWindSpeed * sqrt(dFrequency) * dDistance;
    if (bWindDirection == WIND_DIRECTION_DOWNWIND)
    { dWindLoss *= -0.45; }
    return dWindLoss;

}

/*======================================================================================*/
double MainComponent::dCalculateTemperatureGradientLoss(const double dFrequency, const double dDistance,
    const int nTemperatureGradient)
    /*======================================================================================*/
{
    double dTemperatureGradient = 0;
    if(nTemperatureGradient == TEMPERATURE_LAPSE)
    { dTemperatureGradient = -0.0065; }
    else
    { dTemperatureGradient = 0.001; }

    // Compute attenuation loss based on gradient
    double dTempLoss = -0.03 * dTemperatureGradient * sqrt(dFrequency) * dDistance;
    return dTempLoss;
}

/*======================================================================================*/
double MainComponent::dCalculateAirAttenuationPerMetre(const double dFreq, const double dTemperature, const double dRelativeHumidity, const double dAtmosphericPressure)
// Uses the source of this page : http://www.sengpielaudio.com/calculator-air.htm
/*======================================================================================*/
{
#define TEMP_0DEGS_KELVIN	273.15
#define TEMP_T0				293.15
#define REFERENCE_PRESSURE	101325

    double dPressure = dAtmosphericPressure * 100;
    // Calculate relative pressure
    double dRelativePressure = dPressure / REFERENCE_PRESSURE;
    double dTempKelvin = (dTemperature + TEMP_0DEGS_KELVIN);
    // Calculate the absolute humidity (percentage mole ratio)
    double dAbsoluteHumidity = 4.6151 - (6.8346 * pow((TEMP_0DEGS_KELVIN / dTempKelvin), 1.261));
    double dHumidity = dRelativeHumidity * pow(10, dAbsoluteHumidity) * dRelativePressure;
    // Convert temperature relative to 20 degsC
    double dTempRel = dTempKelvin / TEMP_T0;
    double dFrO = dRelativePressure * (24 + (40400 * dHumidity * ((0.02 + dHumidity) / (0.391 + dHumidity))));
    double dFrN = dRelativePressure * pow(dTempRel, -0.5) * (9 + (280 * dHumidity * exp(-4.17 * (pow(dTempRel, -1 / 3) - 1))));
    double dTemp1 = 0.01275 * (exp(-2239.1 / dTempKelvin) * 1 / (dFrO + ((dFreq * dFreq) / dFrO)));
    double dTemp2 = 0.1068 * (exp(-3352 / dTempKelvin) * 1 / (dFrN + ((dFreq * dFreq) / dFrN)));
    double dTemp3 = (1.84e-11 * (1 / dRelativePressure) * sqrt(dTempRel));
    double dTemp4 = (pow(dTempRel, -2.5) * (dTemp1 + dTemp2));
    double dAlpha = 8.686 * dFreq * dFreq * (dTemp3 + dTemp4);    
    return dAlpha;
}

/*======================================================================================*/
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
/*======================================================================================*/
{
    // Sends audio blocks to various components
    bufferToFill.clearActiveBufferRegion();
    m_cTransportSource.getNextAudioBlock(bufferToFill);

    // Push processed buffer to controls
    AudioBuffer<float> buffer(bufferToFill.buffer->getArrayOfWritePointers(),
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.startSample,
        bufferToFill.numSamples);
    m_pcWaveformDisplay->vPushBuffer(buffer); // Push to waveform visualiser
    m_pcPreProcessingSpectrogram->vPushBuffer(buffer);   // Push to the unproccessed spectrogram
    m_pcPreProcessingFrequencyAnalyser->vPushBuffer(buffer); // Push to the unproccessed frequency spectrum

    vApplyDSPProcessing(buffer);
    m_pcPostProcessingSpectrogram->vPushBuffer(buffer);
    m_pcPostProcessingFrequencyAnalyser->vPushBuffer(buffer);

    // Apply the volume level to the buffer
    float fGain = (float)Decibels::decibelsToGain(m_pcVolumeControl->dGetGain());
    if (bufferToFill.buffer != nullptr)
    {
        for (int nChannel = 0; nChannel < bufferToFill.buffer->getNumChannels(); ++nChannel)
        {
            float* fChannelData = bufferToFill.buffer->getWritePointer(nChannel, bufferToFill.startSample);
            for (int nSample = 0; nSample < bufferToFill.numSamples; ++nSample)
            { fChannelData[nSample] *= fGain; }
        }
    }

    // Calculate rms level to the audio meter
    float fRMSLevel = 0.0f;
    for (int nChannel = 0; nChannel < buffer.getNumChannels(); ++nChannel)
    {
        fRMSLevel += buffer.getRMSLevel(nChannel, bufferToFill.startSample, bufferToFill.numSamples);
    }
    fRMSLevel /= buffer.getNumChannels(); // Average across channels

    // Convert RMS to a meter-friendly scale
    fRMSLevel = jmap(Decibels::gainToDecibels(fRMSLevel), -60.0f, 0.0f, 0.0f, 1.0f);
    fRMSLevel = jlimit(0.0f, 1.0f, fRMSLevel); // Ensure it's within 0-1 range

    // Update the level meter asynchronously to avoid UI issues
    MessageManager::callAsync([this, fRMSLevel] {
        m_pcVolumeControl->vSetMeterLevel(fRMSLevel);
        });
}

/*======================================================================================*/
void MainComponent::releaseResources()
/*======================================================================================*/
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
}

/*======================================================================================*/
void MainComponent::paint(Graphics& g)
/*======================================================================================*/
{
    // Main window background
    g.fillAll(Colours::white);

    // App title
    juce::Rectangle<float> rcTitle(10, 10, 940, 130);
    { g.drawImage(m_iTitleImage, rcTitle, RectanglePlacement::centred, false); }

    g.setColour(Colours::darkseagreen);
    g.fillRect(430, 460, 520, 30);
    g.setFont(20.0f);
    g.setColour(Colours::black);
    g.drawText("Original audio", 430, 460, 520, 30, Justification::centred, true);

    g.setColour(Colours::darkseagreen);
    g.fillRect(430, 660, 520, 30);
    g.setFont(20.0f);
    g.setColour(Colours::black);
    g.drawText("Processed audio", 430, 660, 520, 30, Justification::centred, true);
}

/*======================================================================================*/
void MainComponent::resized()
/*======================================================================================*/
{
    m_pcOpenFileButton->setBounds(670, 150, 120, 25);
    m_pcPlayAudioButton->setBounds(670, 180, 120, 25);
    m_pcPauseAudioButton->setBounds(670, 210, 120, 25);
    m_pcStopAudioButton->setBounds(670, 240, 120, 25);
    m_pcVolumeControl->setBounds(800, 150, 150, 300);
    m_pcWaveformDisplay->setBounds(10, 150, 650, 115);
    m_pcConditionControls->setBounds(10, 460, 410, 190);
    m_pcWeatherPresets->setBounds(670, 275, 120, 175);
    m_pcDistanceGraphic->setBounds(10, 275, 650, 175);
    m_pcPreProcessingSpectrogram->setBounds(430, 490, 520, 160);
    m_pcPostProcessingSpectrogram->setBounds(430, 690, 520, 160);
    m_pcPreProcessingFrequencyAnalyser->setBounds(430, 490, 520, 160);
    m_pcPostProcessingFrequencyAnalyser->setBounds(430, 690, 520, 160);
    m_pcShowSpectrogramButton->setBounds(865, 465, 80, 20);
    m_pcShowSpectrumButton->setBounds(780, 465, 80, 20);
    m_pcResponseCurve->setBounds(10, 660, 410, 190);
}