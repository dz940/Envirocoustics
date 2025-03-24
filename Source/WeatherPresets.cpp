#include "WeatherPresets.h"
#include "MainComponent.h"

/*======================================================================================*/
WeatherPresets::WeatherPresets(MainComponent& pcParentComponent)
    : m_pcMainComponent(pcParentComponent)
/*======================================================================================*/
{
    // Load images assets
    m_iSunOffImage = ImageFileFormat::loadFrom(BinaryData::sunOff_png, BinaryData::sunOff_pngSize);
    m_iSunOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iRainOffImage = ImageFileFormat::loadFrom(BinaryData::rainOff_png, BinaryData::rainOff_pngSize);
    m_iRainOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iWindOffImage = ImageFileFormat::loadFrom(BinaryData::windOff_png, BinaryData::windOff_pngSize);
    m_iWindOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iSnowOffImage = ImageFileFormat::loadFrom(BinaryData::snowOff_png, BinaryData::snowOff_pngSize);
    m_iSnowOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iSunOnImage = ImageFileFormat::loadFrom(BinaryData::sunOn_png, BinaryData::sunOn_pngSize);
    m_iSunOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iRainOnImage = ImageFileFormat::loadFrom(BinaryData::rainOn_png, BinaryData::rainOn_pngSize);
    m_iRainOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iWindOnImage = ImageFileFormat::loadFrom(BinaryData::windOn_png, BinaryData::windOn_pngSize);
    m_iWindOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iSnowOnImage = ImageFileFormat::loadFrom(BinaryData::snowOn_png, BinaryData::snowOn_pngSize);
    m_iSnowOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iSunDownImage = ImageFileFormat::loadFrom(BinaryData::sunDown_png, BinaryData::sunDown_pngSize);
    m_iSunDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iRainDownImage = ImageFileFormat::loadFrom(BinaryData::rainDown_png, BinaryData::rainDown_pngSize);
    m_iRainDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iWindDownImage = ImageFileFormat::loadFrom(BinaryData::windDown_png, BinaryData::windDown_pngSize);
    m_iWindDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    m_iSnowDownImage = ImageFileFormat::loadFrom(BinaryData::snowDown_png, BinaryData::snowDown_pngSize);
    m_iSnowDownImage.rescaled(40, 40, Graphics::highResamplingQuality);

    // Create buttons
    m_pcSnowBtn.onClick = [this] {m_pcMainComponent.vSetParameter(PARAMETER_TEMPERATURE, -10, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRECIPITATION, ON, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRESSURE, 925, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, ON, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_HUMIDITY, 65, true);
                                m_pcMainComponent.vUpdateConditionControls(); };
    m_pcSnowBtn.setClickingTogglesState(true);
    m_pcSnowBtn.setEnabled(true);
    m_pcSnowBtn.setRadioGroupId(4);
    m_pcSnowBtn.setTooltip("Snowy");
    addAndMakeVisible(&m_pcSnowBtn);

    m_pcRainBtn.setEnabled(true);
    m_pcRainBtn.setClickingTogglesState(true);
    m_pcRainBtn.onClick = [this] {m_pcMainComponent.vSetParameter(PARAMETER_TEMPERATURE, 15, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRECIPITATION, ON, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_HUMIDITY, 80, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, ON, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRESSURE, 1013, true);
                                m_pcMainComponent.vUpdateConditionControls(); };
    m_pcRainBtn.setRadioGroupId(4);
    m_pcRainBtn.setTooltip("Rainy");
    addAndMakeVisible(&m_pcRainBtn);

    m_pcWindBtn.onClick = [this] {m_pcMainComponent.vSetParameter(PARAMETER_TEMPERATURE, 15, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_WIND_SPEED, 100, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRECIPITATION, OFF, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, WIND_DIRECTION_UPWIND, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, OFF, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_HUMIDITY, 45, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRESSURE, 950, true);
                                m_pcMainComponent.vUpdateConditionControls(); };
    m_pcWindBtn.setClickingTogglesState(true);
    m_pcWindBtn.setEnabled(true);
    m_pcWindBtn.setRadioGroupId(4);
    m_pcWindBtn.setTooltip("Windy");
    addAndMakeVisible(&m_pcWindBtn);

    m_pcSunBtn.onClick = [this] {m_pcMainComponent.vSetParameter(PARAMETER_TEMPERATURE, 30, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRECIPITATION, OFF, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, OFF, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_HUMIDITY, 50, false);
                                m_pcMainComponent.vSetParameter(PARAMETER_PRESSURE, 1065, true);
                                m_pcMainComponent.vUpdateConditionControls(); };
    m_pcSunBtn.setClickingTogglesState(true);
    m_pcSunBtn.setEnabled(true);
    m_pcSunBtn.setRadioGroupId(4);
    m_pcSunBtn.setTooltip("Sunny");
    m_pcSunBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&m_pcSunBtn);

    // Map the images to the buttons
    m_pcSunBtn.setImages(false, true, true, m_iSunOffImage, 1.0f, Colours::transparentBlack, m_iSunDownImage, 1.0f, Colours::transparentBlack, m_iSunOnImage, 1.0f, Colours::transparentBlack);
    m_pcRainBtn.setImages(false, true, true, m_iRainOffImage, 1.0f, Colours::transparentBlack, m_iRainDownImage, 1.0f, Colours::transparentBlack, m_iRainOnImage, 1.0f, Colours::transparentBlack);
    m_pcWindBtn.setImages(false, true, true, m_iWindOffImage, 1.0f, Colours::transparentBlack, m_iWindDownImage, 1.0f, Colours::transparentBlack, m_iWindOnImage, 1.0f, Colours::transparentBlack);
    m_pcSnowBtn.setImages(false, true, true, m_iSnowOffImage, 1.0f, Colours::transparentBlack, m_iSnowDownImage, 1.0f, Colours::transparentBlack, m_iSnowOnImage, 1.0f, Colours::transparentBlack);
}

/*======================================================================================*/
WeatherPresets::~WeatherPresets()
/*======================================================================================*/
{
}

/*======================================================================================*/
void WeatherPresets::resized()
/*======================================================================================*/
{
    Rectangle<int> rcWeatherPresets = getLocalBounds();

    int nButtonWidth = (rcWeatherPresets.getWidth() - 15) / 2;
    m_pcSunBtn.setBounds(rcWeatherPresets.getX() + 5, rcWeatherPresets.getY() + 60, nButtonWidth, nButtonWidth);
    m_pcWindBtn.setBounds(rcWeatherPresets.getX() + 10 + nButtonWidth, rcWeatherPresets.getY() + 60, nButtonWidth, nButtonWidth);
    m_pcRainBtn.setBounds(rcWeatherPresets.getX() + 5, rcWeatherPresets.getY() + 65 + nButtonWidth, nButtonWidth, nButtonWidth);
    m_pcSnowBtn.setBounds(rcWeatherPresets.getX() + 10 + nButtonWidth, rcWeatherPresets.getY() + 65 + nButtonWidth, nButtonWidth, nButtonWidth);
}

/*======================================================================================*/
void WeatherPresets::paint(Graphics& g)
/*======================================================================================*/
{
    g.setColour(Colours::darkseagreen);
    Rectangle<int> rcWeatherPresets = getLocalBounds(); // x, y, width, height
    g.fillRect(rcWeatherPresets);

    g.setFont(20.0f);
    g.setColour(Colours::black);
    Rectangle<int> rcWeatherPresetsTitle1(rcWeatherPresets.getX(),
        rcWeatherPresets.getY(),
        rcWeatherPresets.getWidth(),
        25);
    g.drawText("Weather", rcWeatherPresetsTitle1, Justification::centred, true);
    Rectangle<int> rcWeatherPresetsTitle2(rcWeatherPresets.getX(),
        rcWeatherPresets.getY() + 25,
        rcWeatherPresets.getWidth(),
        25);
    g.drawText("presets", rcWeatherPresetsTitle2, Justification::centred, true);
}
