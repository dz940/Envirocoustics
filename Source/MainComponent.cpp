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
#include "WindAttenuation.h"
#include "TempGradientAttenuation.h"
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

    m_pcDisablePowerLossCheck = new ToggleButton();
    m_pcDisablePowerLossCheck->setButtonText("Enable Make-up Gain");
    m_pcDisablePowerLossCheck->onClick = [this] { if (m_pcDisablePowerLossCheck->getToggleState() == ON) { vShowVolumeWarningBox(); }
                                                  else { vSetParameter(PARAMETER_MAKEUP_GAIN, 0, false); }; };
    m_pcDisablePowerLossCheck->setColour(ToggleButton::tickColourId, Colours::black);
    m_pcDisablePowerLossCheck->setColour(ToggleButton::textColourId, Colours::black);
    m_pcDisablePowerLossCheck->setColour(ToggleButton::tickDisabledColourId, Colours::black);
    m_pcDisablePowerLossCheck->setToggleState(true, dontSendNotification);
    m_pcDisablePowerLossCheck->setEnabled(true);
    addAndMakeVisible(m_pcDisablePowerLossCheck);

    m_cFormatManager.registerBasicFormats();
    m_cTransportSource.addChangeListener(this);

    m_cMainFilterL.reset();
    m_cMainFilterR.reset();
    m_cWindFilterL.reset();
    m_cWindFilterR.reset();
    m_cTempFilterL.reset();
    m_cTempFilterR.reset();

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
    delete m_pcDisablePowerLossCheck;
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
void MainComponent::vSetParameter(const int nParameterType, const double dValue, const bool bUpdateDisplay)
/*======================================================================================*/
{
    // Centrally setting paramaters from all the controls
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        {
            m_dTemperature = dValue;
            vUpdateMainFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_HUMIDITY:
        {
            m_dHumidity = dValue;
            vUpdateMainFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_WIND_SPEED:
        {
            m_dWindSpeed = dValue;
            vUpdateWindFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            m_bWindDirection = (bool)dValue;
            vUpdateWindFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_PRECIPITATION:
        {
            m_bPrecipitation = (bool)dValue;

            // Precipitation automatically sets humidity to 90%
            if (m_bPrecipitation)
            { m_dHumidity = 90; }
            else
            { m_dHumidity = 50; }
            vUpdateMainFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            m_bTempGradient = (bool)dValue;
            vUpdateTempFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_PRESSURE:
        {
            m_dPressure = dValue;
            vUpdateMainFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_CLOUD_COVER:
        {
            m_bCloudCover = (bool)dValue;
            break;
        }
        case PARAMETER_DISTANCE:
        {
            m_dDistance = dValue;
            vUpdateWindFilterCoeffs();
            vUpdateTempFilterCoeffs();
            vUpdateMainFilterCoeffs();
            vUpdateSystemResponse();
            break;
        }
        case PARAMETER_MAKEUP_GAIN:
        {
            m_bEnableMakeupGain = dValue;
            vUpdateSystemResponse();
        }
    }
    if(bUpdateDisplay)
    { m_pcDistanceGraphic->repaint(); }
}

/*======================================================================================*/
void MainComponent::vUpdateWindFilterCoeffs()
/*======================================================================================*/
{
    WindFilterCutoffSolver cWindSolver(m_dWindSpeed, m_bWindDirection);
    if (m_bWindDirection == WIND_DIRECTION_UPWIND)
    {
        double dWindCutoffFreq = cWindSolver.SolveUpwind(m_dDistance);
        m_cWindIIRCoeffs = IIRCoefficients::makeLowPass(44100, dWindCutoffFreq);
        m_cWindFilterL.setCoefficients(m_cWindIIRCoeffs);
        m_cWindFilterR.setCoefficients(m_cWindIIRCoeffs);

        dsp::IIR::Coefficients<float>::Ptr fpNewCoeffs = dsp::IIR::Coefficients<float>::makeLowPass(44100, (float)dWindCutoffFreq);
        m_pcResponseCurve->vSetWindCoefficients(fpNewCoeffs);
    }
    else
    {
        double dWindHighShelfBoost = cWindSolver.SolveDownwind(m_dDistance);
        m_cWindIIRCoeffs = IIRCoefficients::makePeakFilter(44100, 5000.0, 0.2, (float)dWindHighShelfBoost);
        m_cWindFilterL.setCoefficients(m_cWindIIRCoeffs);
        m_cWindFilterR.setCoefficients(m_cWindIIRCoeffs);

        dsp::IIR::Coefficients<float>::Ptr fpNewCoeffs = dsp::IIR::Coefficients<float>::makePeakFilter(44100, 5000.0f, 0.2f, (float)dWindHighShelfBoost);
        m_pcResponseCurve->vSetWindCoefficients(fpNewCoeffs);
    }
}

/*======================================================================================*/
void MainComponent::vUpdateTempFilterCoeffs()
/*======================================================================================*/
{
    TempGradientFilterCutoffSolver cTempGradientSolver(m_bTempGradient);
    if (m_bTempGradient == TEMPERATURE_LAPSE)
    {
        double dTempCutoffFreq = cTempGradientSolver.SolveLapse(m_dDistance);
        m_cTempIIRCoeffs = IIRCoefficients::makeLowPass(44100, dTempCutoffFreq);
        m_cTempFilterL.setCoefficients(m_cTempIIRCoeffs);
        m_cTempFilterR.setCoefficients(m_cTempIIRCoeffs);

        dsp::IIR::Coefficients<float>::Ptr fpNewCoeffs = dsp::IIR::Coefficients<float>::makeLowPass(44100, (float)dTempCutoffFreq);
        m_pcResponseCurve->vSetTempGradientCoefficients(fpNewCoeffs);
    }
    else
    {
        double dTempBoost = cTempGradientSolver.SolveInversion(m_dDistance);
        m_cTempIIRCoeffs = IIRCoefficients::makePeakFilter(44100, 5000.0, 0.2, (float)dTempBoost);
        m_cTempFilterL.setCoefficients(m_cTempIIRCoeffs);
        m_cTempFilterR.setCoefficients(m_cTempIIRCoeffs);

        dsp::IIR::Coefficients<float>::Ptr fpNewCoeffs = dsp::IIR::Coefficients<float>::makePeakFilter(44100, 5000.0f, 0.2f, (float)dTempBoost);
        m_pcResponseCurve->vSetTempGradientCoefficients(fpNewCoeffs);
    }
}

/*======================================================================================*/
void MainComponent::vUpdateMainFilterCoeffs()
/*======================================================================================*/
{
    // Initialise the cutoff solver with atmospheric conditions
    double dTempFarenheit = (m_dTemperature * 1.8) + 32; // Atmospheric absorption calculation uses farenheit
    double dPressurePascals = m_dPressure * 100; // Atmoshperic absoption calculation uses pascals
    FilterCutoffSolver cCutoffSolver(m_dHumidity, dTempFarenheit, dPressurePascals);

    double dDistance = (m_dDistance == 0) ? 1 : m_dDistance; // Set minimum distance to 1 so that it doesnt break the calculation
    double dCutoffFrequency = cCutoffSolver.Solve(dDistance); // Calculate the LPF cutoff frequency
    dCutoffFrequency = (dCutoffFrequency > 22000.0) ? 20000.0 : dCutoffFrequency; // Ensure cut off frequency is not greater than 20kHz

    // Set the low-pass filter with the calculated cutoff frequency
    m_cMainIIRCoeffs = IIRCoefficients::makeLowPass(44100, dCutoffFrequency);
    m_cMainFilterL.setCoefficients(m_cMainIIRCoeffs);
    m_cMainFilterR.setCoefficients(m_cMainIIRCoeffs);
}

/*======================================================================================*/
void MainComponent::vUpdateConditionControls()
/*======================================================================================*/
{
    // Complete update and repaint of condition controls
    m_pcConditionControls->vSetParameter(PARAMETER_TEMPERATURE, m_dTemperature);
    m_pcConditionControls->vSetParameter(PARAMETER_WIND_SPEED, m_dWindSpeed);
    m_pcConditionControls->vSetParameter(PARAMETER_HUMIDITY, m_dHumidity);
    m_pcConditionControls->vSetParameter(PARAMETER_TEMP_GRADIENT, m_bTempGradient);
    m_pcConditionControls->vSetParameter(PARAMETER_PRECIPITATION, m_bPrecipitation);
    m_pcConditionControls->vSetParameter(PARAMETER_WIND_DIRECTION, m_bWindDirection);
    m_pcConditionControls->vSetParameter(PARAMETER_PRESSURE, m_dPressure);
    m_pcConditionControls->vSetParameter(PARAMETER_CLOUD_COVER, m_bCloudCover);
    m_pcConditionControls->repaint();
}

/*======================================================================================*/
double MainComponent::dGetParameter(const int nParameterType)
/*======================================================================================*/
{
    // Retrieving parameters from central location
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        { return m_dTemperature; }
        case PARAMETER_HUMIDITY:
        { return m_dHumidity; }
        case PARAMETER_WIND_SPEED:
        { return m_dWindSpeed; }
        case PARAMETER_WIND_DIRECTION:
        { return (int)m_bWindDirection; }
        case PARAMETER_PRECIPITATION:
        { return (int)m_bPrecipitation; }
        case PARAMETER_TEMP_GRADIENT:
        {  return (int)m_bTempGradient; }
        case PARAMETER_PRESSURE:
        { return m_dPressure; }
        case PARAMETER_CLOUD_COVER:
        { return (int)m_bCloudCover; }
        case PARAMETER_DISTANCE:
        { return m_dDistance; }
        case PARAMETER_MAKEUP_GAIN:
        { return (int)m_bEnableMakeupGain; }
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
                vTransportSourceStateChanged(Stopped);
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
        if (!m_cTransportSource.isPlaying() && m_cTransportSourceState != Stopped)
        { vTransportSourceStateChanged(Stopping); }
    }
}

/*======================================================================================*/
void MainComponent::vApplyDSPProcessing(AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    int nNumSamples = buffer.getNumSamples();
    float* pfChannelDataL = buffer.getWritePointer(0);
    float* pfChannelDataR = buffer.getWritePointer(1);
    double dDistance = (m_dDistance == 0) ? 1 : m_dDistance;

    //**************Atmospheric absorption**************//
    m_cMainFilterL.processSamples(pfChannelDataL, nNumSamples);
    m_cMainFilterR.processSamples(pfChannelDataR, nNumSamples);

    //**************Wind scattering and refraction**************//
    m_cWindFilterL.processSamples(pfChannelDataL, nNumSamples);
    m_cWindFilterR.processSamples(pfChannelDataR, nNumSamples);

    //**************Temperature gradients**************//
    m_cTempFilterL.processSamples(pfChannelDataL, nNumSamples);
    m_cTempFilterR.processSamples(pfChannelDataR, nNumSamples);

    //**************Geometric spreading**************//
    // Apply Inverse Square Law attenuation (linear gain)
    if (!m_bEnableMakeupGain)
    {
        float fGain = 1.0f / static_cast<float>(dDistance); // Linear gain (1/distance)
        buffer.applyGain(fGain);
    }
}

/*======================================================================================*/
void MainComponent::vUpdateSystemResponse()
/*======================================================================================*/
{
    m_pcResponseCurve->repaint();
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
void MainComponent::vShowVolumeWarningBox()
/*======================================================================================*/
{
    // Use callAsync to show the dialog asynchronously on the message thread
    juce::MessageManager::callAsync([this]() {
        // Ensure that the callback captures 'this' to access member variables/functions
        const auto callback = juce::ModalCallbackFunction::create([this](int nResult) {
            if (nResult == 1) {
                vSetParameter(PARAMETER_MAKEUP_GAIN, 1, false);
            }
            else {
                m_pcDisablePowerLossCheck->setToggleState(false, juce::dontSendNotification);
            }
            });

        // Display the modal AlertWindow with "OK" and "Cancel"
        juce::AlertWindow::showOkCancelBox(
            juce::MessageBoxIconType::WarningIcon,  // Icon
            "Warning",  // Window Title
            "Please lower your volume before proceeding to avoid potential damage to your speakers/headphones.",  // Message
            "Proceed",  // Button text for OK
            "Go back",  // Button text for Cancel
            nullptr,  // Component to attach (optional)
            callback);  // The callback function for OK/Cancel actions
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
    m_pcDisablePowerLossCheck->setBounds(430, 465, 100, 20);
}