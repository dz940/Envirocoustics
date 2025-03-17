#include "MainComponent.h"
#include "WeatherPresets.h"
#include "DistanceGraphic.h"
#include "WaveformDisplay.h"
#include "ConditionControls.h"
#include "SpectrumComponent.h"
#include "VolumeControl.h"
#include "SpectrogramComponent.h"
#include "ResponseCurve.h"
#include <atlstr.h>

/*======================================================================================*/
MainComponent::MainComponent() : juce::AudioAppComponent(customDeviceManager)
/*======================================================================================*/
{
    // Load logo
    titleImage = ImageFileFormat::loadFrom(BinaryData::title_png, BinaryData::title_pngSize);
    setOpaque(true);
    lowPassFilter.reset();

    // Creating all components
    distanceGraphic = new DistanceGraphic(*this);
    addAndMakeVisible(distanceGraphic);
    distanceGraphic->setOpaque(true);

    weatherPresets = new WeatherPresets(*this);
    addAndMakeVisible(weatherPresets);
    weatherPresets->setOpaque(true);

    waveformDisplay = new WaveformDisplay(*this);
    addAndMakeVisible(waveformDisplay);
    waveformDisplay->setOpaque(true);

    conditionControls = new ConditionControls(*this);
    addAndMakeVisible(conditionControls);
    conditionControls->setOpaque(true);

    volumeControl = new VolumeControl(*this);
    addAndMakeVisible(volumeControl);
    volumeControl->setOpaque(true);

    spectrogram1 = new SpectrogramComponent(*this);
    addAndMakeVisible(spectrogram1);
    spectrogram1->setOpaque(true);
    spectrogram1->setVisible(false);

    spectrogram2 = new SpectrogramComponent(*this);
    addAndMakeVisible(spectrogram2);
    spectrogram2->setOpaque(true);
    spectrogram2->setVisible(false);

    frequencyAnalyser1 = new SpectrumComponent(*this);
    addAndMakeVisible(frequencyAnalyser1);
    frequencyAnalyser1->setOpaque(true);

    frequencyAnalyser2 = new SpectrumComponent(*this);
    addAndMakeVisible(frequencyAnalyser2);
    frequencyAnalyser2->setOpaque(true);

    responseCurve = new ResponseCurve(*this);
    addAndMakeVisible(responseCurve);
    responseCurve->setOpaque(true);

    // Initialising the transport source state
    transportSourceState = Stopped;
    customDeviceManager.initialise(0, 2, nullptr, true);
    setAudioChannels(0, 2); // Stereo audio

    // Setting up the main component buttons
    openFileButton.setButtonText("Open File");
    openFileButton.onClick = [this] {openTextButtonClicked(); };
    openFileButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkseagreen);
    addAndMakeVisible(&openFileButton);

    playAudioButton.setButtonText("Play");
    playAudioButton.onClick = [this] {playAudioButtonClicked(); };
    playAudioButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkseagreen);
    playAudioButton.setEnabled(false);
    addAndMakeVisible(&playAudioButton);

    pauseAudioButton.setButtonText("Pause");
    pauseAudioButton.onClick = [this] {pauseAudioButtonClicked(); };
    pauseAudioButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkseagreen);
    pauseAudioButton.setEnabled(false);
    addAndMakeVisible(&pauseAudioButton);

    stopAudioButton.setButtonText("Stop");
    stopAudioButton.onClick = [this] {stopAudioButtonClicked(); };
    stopAudioButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkseagreen);
    stopAudioButton.setEnabled(false);
    addAndMakeVisible(&stopAudioButton);

    spectrumButton.setButtonText("Spectrum");
    spectrumButton.onClick = [this] {switchToSpectrum(); };
    spectrumButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    spectrumButton.setEnabled(true);
    spectrumButton.setClickingTogglesState(true);
    spectrumButton.setRadioGroupId(1);
    spectrumButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&spectrumButton);

    spectrogramButton.setButtonText("Spectrogram");
    spectrogramButton.onClick = [this] {switchToSpectrogram(); };
    spectrogramButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    spectrogramButton.setEnabled(true);
    spectrogramButton.setClickingTogglesState(true);
    spectrogramButton.setRadioGroupId(1);
    addAndMakeVisible(&spectrogramButton);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    tooltipWindow.setMillisecondsBeforeTipAppears(500);
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
    delete distanceGraphic;
    delete weatherPresets;
    delete waveformDisplay;
    delete conditionControls;
    delete volumeControl;
    delete responseCurve;
    delete spectrogram1;
    delete spectrogram2;
    delete frequencyAnalyser1;
    delete frequencyAnalyser2;
}

/*======================================================================================*/
void MainComponent::switchToSpectrogram()
/*======================================================================================*/
{
    // Hide frequency spectrum and show spectrogram
    frequencyAnalyser1->setVisible(false);
    frequencyAnalyser2->setVisible(false);
    spectrogram1->setVisible(true);
    spectrogram2->setVisible(true);
}

/*======================================================================================*/
void MainComponent::switchToSpectrum()
/*======================================================================================*/
{
    // Hide spectrogram and show frequency spectrum
    spectrogram1->setVisible(false);
    spectrogram2->setVisible(false);
    frequencyAnalyser1->setVisible(true);
    frequencyAnalyser2->setVisible(true);
}

/*======================================================================================*/
void MainComponent::vSetParameter(int nParameterType, int nValue, bool bUpdateDisplay)
/*======================================================================================*/
{
    // Centrally setting paramaters from all the controls
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        {
            nTemperature = nValue; 
            updateSystemResponse();
            break;
        }
        case PARAMETER_HUMIDITY:
        {
            nHumidity = nValue;
            updateSystemResponse();
            break;
        }
        case PARAMETER_WIND_SPEED:
        {
            nWindSpeed = nValue;
            updateSystemResponse();
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            nWindDirection = nValue;
            updateSystemResponse();
            break;
        }
        case PARAMETER_PRECIPITATION:
        {
            bPrecipitation = (bool)nValue;

            // Precipitation automatically sets humidity to 90%
            if (bPrecipitation)
            { nHumidity = 90; }
            else
            { nHumidity = 50; }
            updateSystemResponse();
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            nTempGradient = nValue;
            updateSystemResponse();
            break;
        }
        case PARAMETER_PRESSURE:
        {
            nPressure = nValue;
            updateSystemResponse();
            break;
        }
        case PARAMETER_CLOUD_COVER:
        {
            bCloudCover = (bool)nValue;
            break;
        }
        case PARAMETER_DISTANCE:
        {
            nDistance = nValue;
            updateSystemResponse();
            //updateFilter(nDistance);
            break;
        }
    }
    if(bUpdateDisplay)
    { distanceGraphic->repaint(); }
}

/*======================================================================================*/
void MainComponent::vUpdateConditionControls()
/*======================================================================================*/
{
    // Complete update and repaint of condition controls
    conditionControls->vSetParameter(PARAMETER_TEMPERATURE, nTemperature);
    conditionControls->vSetParameter(PARAMETER_WIND_SPEED, nWindSpeed);
    conditionControls->vSetParameter(PARAMETER_HUMIDITY, nHumidity);
    conditionControls->vSetParameter(PARAMETER_TEMP_GRADIENT, nTempGradient);
    conditionControls->vSetParameter(PARAMETER_PRECIPITATION, bPrecipitation);
    conditionControls->vSetParameter(PARAMETER_WIND_DIRECTION, nWindDirection);
    conditionControls->vSetParameter(PARAMETER_PRESSURE, nPressure);
    conditionControls->vSetParameter(PARAMETER_CLOUD_COVER, bCloudCover);
    conditionControls->repaint();
}

/*======================================================================================*/
int MainComponent::nGetParameter(int nParameterType)
/*======================================================================================*/
{
    // Retrieving parameters from central location
    switch (nParameterType)
    {
        case PARAMETER_TEMPERATURE:
        { return nTemperature; }
        case PARAMETER_HUMIDITY:
        { return nHumidity; }
        case PARAMETER_WIND_SPEED:
        { return nWindSpeed; }
        case PARAMETER_WIND_DIRECTION:
        { return nWindDirection; }
        case PARAMETER_PRECIPITATION:
        { return (int)bPrecipitation; }
        case PARAMETER_TEMP_GRADIENT:
        {  return nTempGradient; }
        case PARAMETER_PRESSURE:
        { return nPressure; }
        case PARAMETER_CLOUD_COVER:
        { return bCloudCover; }
        case PARAMETER_DISTANCE:
        { return nDistance; }
    }
    jassert(false);
    return 0;
}

/*======================================================================================*/
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
/*======================================================================================*/
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/*======================================================================================*/
void MainComponent::openTextButtonClicked()
/*======================================================================================*/
{
    // Choose file to load and import into the audio player
    chooser = std::make_unique<juce::FileChooser>(
        "Please select the mp3 you want to load...",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.mp3;*.wav");

    auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    // Launch file browser
    chooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File audioFile(chooser.getResult());
            transportSource.stop();
            transportSource.setSource(nullptr);  

            if (auto* reader = formatManager.createReaderFor(audioFile))
            {
                DBG("Loaded file: " + audioFile.getFullPathName());
                playSource.reset(new juce::AudioFormatReaderSource(reader, true));
                transportSource.setSource(playSource.get(), 0, nullptr, reader->sampleRate);
                transportSource.setPosition(0.0);
                playAudioButton.setEnabled(true);
            }
            else
            { DBG("Failed to load file: " + audioFile.getFullPathName()); }
        });
}

/*======================================================================================*/
void MainComponent::playAudioButtonClicked()
/*======================================================================================*/
{
    transportSourceStateChanged(Starting);
}

/*======================================================================================*/
void MainComponent::stopAudioButtonClicked()
/*======================================================================================*/
{
    transportSourceStateChanged(Stopping);
}

/*======================================================================================*/
void MainComponent::pauseAudioButtonClicked()
/*======================================================================================*/
{
    transportSourceStateChanged(Pausing);
}

/*======================================================================================*/
void MainComponent::transportSourceStateChanged(transportSourceState_t state)
/*======================================================================================*/
{
    if (state != transportSourceState)
    {
        transportSourceState = state;

        switch (transportSourceState)
        {
            case Stopped:
            {
                double position = transportSource.getCurrentPosition();
                double length = transportSource.getLengthInSeconds();
                if (std::abs(position - length) < 0.01)
                { transportSource.setPosition(0.0); }
                break;
            }
            case Playing:
            {
                playAudioButton.setEnabled(false);
                pauseAudioButton.setEnabled(true);
                stopAudioButton.setEnabled(true);
                openFileButton.setEnabled(false);
                break;
            }
            case Starting:
            {
                playAudioButton.setEnabled(false);
                openFileButton.setEnabled(false);
                pauseAudioButton.setEnabled(true);
                stopAudioButton.setEnabled(true);
                transportSource.start();
                break;
            }
            case Pausing:
            {
                playAudioButton.setEnabled(true);
                stopAudioButton.setEnabled(true);
                openFileButton.setEnabled(false);
                transportSource.stop();
                break;
            }
            case Stopping:
            {
                playAudioButton.setEnabled(true);
                stopAudioButton.setEnabled(false);
                pauseAudioButton.setEnabled(false);
                openFileButton.setEnabled(true);
                transportSource.stop();
                transportSource.setPosition(0.0);
                break;
            }
        }
    }
}

/*======================================================================================*/
void MainComponent::thumbnailChanged()
/*======================================================================================*/
{
    //repaint();
}

/*======================================================================================*/
void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
/*======================================================================================*/
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
        { transportSourceStateChanged(Playing); }
        else
        { transportSourceStateChanged(Stopped); }
    }
}

/*======================================================================================*/
void MainComponent::DSPEngine(juce::AudioBuffer<float>& buffer)
/*======================================================================================*/
{
    int numSamples = buffer.getNumSamples();

    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);

    for (int i = 0; i < numSamples; i++)
    {
        channelDataL[i] = lowPassFilter.processSample(channelDataL[i]);
        channelDataR[i] = lowPassFilter.processSample(channelDataR[i]);
    }
}

/*======================================================================================*/
void MainComponent::updateSystemResponse()
/*======================================================================================*/
{
    responseCurve->repaint();
}

/*======================================================================================*/
void MainComponent::updateFilter(const int nDistanceVal)
/*======================================================================================*/
{
    float cutOff = 5030.0f - (float)(5 * nDistanceVal);
    lowPassFilter.coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, cutOff, 0.5f);
    responseCurve->setCoefficients(lowPassFilter.coefficients); // Update the curve
}


/*======================================================================================*/
double MainComponent::dCalculateWindLoss(const double dFrequency, const double dDistance, const double dTemperature, const double dWindSpeed, const bool bWindDirection)
/*======================================================================================*/
{
    // Wind attenuation loss
    double windLoss = 1.0e-5 * dWindSpeed * sqrt(dFrequency) * dDistance;
    if (bWindDirection == WIND_DIRECTION_DOWNWIND)
    { windLoss *= -0.45; }
    return windLoss;

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
    double tempLoss = -0.03 * dTemperatureGradient * sqrt(dFrequency) * dDistance;

    return tempLoss;
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
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
/*======================================================================================*/
{
    // Sends audio blocks to various components
    bufferToFill.clearActiveBufferRegion();
    transportSource.getNextAudioBlock(bufferToFill);

    // Push processed buffer to controls
    juce::AudioBuffer<float> buffer(bufferToFill.buffer->getArrayOfWritePointers(),
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.startSample,
        bufferToFill.numSamples);
    waveformDisplay->pushBuffer(buffer); // Push to waveform visualiser
    spectrogram1->pushBuffer(buffer);   // Push to the unproccessed spectrogram
    frequencyAnalyser1->pushBuffer(buffer); // Push to the unproccessed frequency spectrum

    DSPEngine(buffer);
    spectrogram2->pushBuffer(buffer);
    frequencyAnalyser2->pushBuffer(buffer);

    // Apply the volume level to the buffer
    float gain = (float)juce::Decibels::decibelsToGain(volumeControl->getGain());
    if (bufferToFill.buffer != nullptr)
    {
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            { channelData[sample] *= gain; }
        }
    }

    // Calculate rms level to the audio meter
    float rmsLevel = 0.0f;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        rmsLevel += buffer.getRMSLevel(channel, bufferToFill.startSample, bufferToFill.numSamples);
    }
    rmsLevel /= buffer.getNumChannels(); // Average across channels

    // Convert RMS to a meter-friendly scale
    rmsLevel = juce::jmap(juce::Decibels::gainToDecibels(rmsLevel), -60.0f, 0.0f, 0.0f, 1.0f);
    rmsLevel = juce::jlimit(0.0f, 1.0f, rmsLevel); // Ensure it's within 0-1 range

    // Update the level meter asynchronously to avoid UI issues
    juce::MessageManager::callAsync([this, rmsLevel] {
        volumeControl->setMeterLevel(rmsLevel);
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
void MainComponent::paint(juce::Graphics& g)
/*======================================================================================*/
{
    // Main window background
    g.fillAll(juce::Colours::white);

    // App title
    g.setColour(juce::Colours::darkolivegreen);
    g.fillRect(10, 10, 940, 130);
    g.setFont(100.0f);
    g.setColour(juce::Colours::white);
    g.drawText("ENVIROCOUSTICS", 10, 10, 940, 130, juce::Justification::centred, true);

    juce::Rectangle<float> rcTitle(10, 10, 940, 130);
    { g.drawImage(titleImage, rcTitle, RectanglePlacement::centred, false); }

    // Reponse curves
    g.setColour(juce::Colours::darkseagreen);
    g.fillRect(10, 660, 410, 190);
    g.setFont(20.0f);
    g.setColour(juce::Colours::black);
    g.drawText("System response curves", 10, 660, 410, 30, juce::Justification::centred, true);

    g.setColour(juce::Colours::darkseagreen);
    g.fillRect(430, 460, 520, 30);
    g.setFont(20.0f);
    g.setColour(juce::Colours::black);
    g.drawText("Original audio", 430, 460, 520, 30, juce::Justification::centred, true);

    g.setColour(juce::Colours::darkseagreen);
    g.fillRect(430, 660, 520, 30);
    g.setFont(20.0f);
    g.setColour(juce::Colours::black);
    g.drawText("Processed audio", 430, 660, 520, 30, juce::Justification::centred, true);
}

/*======================================================================================*/
void MainComponent::resized()
/*======================================================================================*/
{
    openFileButton.setBounds(670, 150, 120, 25);
    playAudioButton.setBounds(670, 180, 120, 25);
    pauseAudioButton.setBounds(670, 210, 120, 25);
    stopAudioButton.setBounds(670, 240, 120, 25);
    volumeControl->setBounds(800, 150, 150, 300);
    waveformDisplay->setBounds(10, 150, 650, 115);
    conditionControls->setBounds(10, 460, 410, 190);
    weatherPresets->setBounds(670, 275, 120, 175);
    distanceGraphic->setBounds(10, 275, 650, 175);
    spectrogram1->setBounds(430, 490, 520, 160);
    spectrogram2->setBounds(430, 690, 520, 160);
    frequencyAnalyser1->setBounds(430, 490, 520, 160);
    frequencyAnalyser2->setBounds(430, 690, 520, 160);
    spectrogramButton.setBounds(865, 465, 80, 20);
    spectrumButton.setBounds(780, 465, 80, 20);
    responseCurve->setBounds(10, 660, 410, 190);
}