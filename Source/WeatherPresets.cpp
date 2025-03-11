#include "WeatherPresets.h"
#include "MainComponent.h"

/*======================================================================================*/
WeatherPresets::WeatherPresets(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    // Load images assets
    sunOffImage = ImageFileFormat::loadFrom(BinaryData::sunOff_png, BinaryData::sunOff_pngSize);
    sunOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    rainOffImage = ImageFileFormat::loadFrom(BinaryData::rainOff_png, BinaryData::rainOff_pngSize);
    rainOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    windOffImage = ImageFileFormat::loadFrom(BinaryData::windOff_png, BinaryData::windOff_pngSize);
    windOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    snowOffImage = ImageFileFormat::loadFrom(BinaryData::snowOff_png, BinaryData::snowOff_pngSize);
    snowOffImage.rescaled(40, 40, Graphics::highResamplingQuality);
    sunOnImage = ImageFileFormat::loadFrom(BinaryData::sunOn_png, BinaryData::sunOn_pngSize);
    sunOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    rainOnImage = ImageFileFormat::loadFrom(BinaryData::rainOn_png, BinaryData::rainOn_pngSize);
    rainOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    windOnImage = ImageFileFormat::loadFrom(BinaryData::windOn_png, BinaryData::windOn_pngSize);
    windOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    snowOnImage = ImageFileFormat::loadFrom(BinaryData::snowOn_png, BinaryData::snowOn_pngSize);
    snowOnImage.rescaled(40, 40, Graphics::highResamplingQuality);
    sunDownImage = ImageFileFormat::loadFrom(BinaryData::sunDown_png, BinaryData::sunDown_pngSize);
    sunDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    rainDownImage = ImageFileFormat::loadFrom(BinaryData::rainDown_png, BinaryData::rainDown_pngSize);
    rainDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    windDownImage = ImageFileFormat::loadFrom(BinaryData::windDown_png, BinaryData::windDown_pngSize);
    windDownImage.rescaled(40, 40, Graphics::highResamplingQuality);
    snowDownImage = ImageFileFormat::loadFrom(BinaryData::snowDown_png, BinaryData::snowDown_pngSize);
    snowDownImage.rescaled(40, 40, Graphics::highResamplingQuality);

    // Create buttons
    snowBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, -10, false);
                                mainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, ON, false);
                                mainComponent.vSetParameter(PARAMETER_PRESSURE, 925, false);
                                mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, ON, false);
                                mainComponent.vSetParameter(PARAMETER_HUMIDITY, 65, true);
                                mainComponent.vUpdateConditionControls(); };
    snowBtn.setClickingTogglesState(true);
    snowBtn.setEnabled(true);
    snowBtn.setRadioGroupId(4);
    addAndMakeVisible(&snowBtn);

    rainBtn.setEnabled(true);
    rainBtn.setClickingTogglesState(true);
    rainBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, 15, false);
                                mainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, ON, false);
                                mainComponent.vSetParameter(PARAMETER_HUMIDITY, 80, false);
                                mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, ON, false);
                                mainComponent.vSetParameter(PARAMETER_PRESSURE, 1013, true);
                                mainComponent.vUpdateConditionControls(); };
    rainBtn.setRadioGroupId(4);
    addAndMakeVisible(&rainBtn);

    windBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, 15, false);
                                mainComponent.vSetParameter(PARAMETER_WIND_SPEED, 100, false);
                                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, OFF, false);
                                mainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, WIND_DIRECTION_LEFT, false);
                                mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, OFF, false);
                                mainComponent.vSetParameter(PARAMETER_HUMIDITY, 45, false);
                                mainComponent.vSetParameter(PARAMETER_PRESSURE, 950, true);
                                mainComponent.vUpdateConditionControls(); };
    windBtn.setClickingTogglesState(true);
    windBtn.setEnabled(true);
    windBtn.setRadioGroupId(4);
    addAndMakeVisible(&windBtn);

    sunBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, 30, false);
                                mainComponent.vSetParameter(PARAMETER_WIND_SPEED, 0, false);
                                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, OFF, false);
                                mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, OFF, false);
                                mainComponent.vSetParameter(PARAMETER_HUMIDITY, 50, false);
                                mainComponent.vSetParameter(PARAMETER_PRESSURE, 1065, true);
                                mainComponent.vUpdateConditionControls(); };
    sunBtn.setClickingTogglesState(true);
    sunBtn.setEnabled(true);
    sunBtn.setRadioGroupId(4);
    sunBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&sunBtn);

    // Map the images to the buttons
    sunBtn.setImages(false, true, true, sunOffImage, 1.0f, Colours::transparentBlack, sunDownImage, 1.0f, Colours::transparentBlack, sunOnImage, 1.0f, Colours::transparentBlack);
    rainBtn.setImages(false, true, true, rainOffImage, 1.0f, Colours::transparentBlack, rainDownImage, 1.0f, Colours::transparentBlack, rainOnImage, 1.0f, Colours::transparentBlack);
    windBtn.setImages(false, true, true, windOffImage, 1.0f, Colours::transparentBlack, windDownImage, 1.0f, Colours::transparentBlack, windOnImage, 1.0f, Colours::transparentBlack);
    snowBtn.setImages(false, true, true, snowOffImage, 1.0f, Colours::transparentBlack, snowDownImage, 1.0f, Colours::transparentBlack, snowOnImage, 1.0f, Colours::transparentBlack);
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
    Rectangle rect = getLocalBounds();
    Rectangle<int> rcWeatherPresets = rect;

    int nButtonWidth = (rcWeatherPresets.getWidth() - 15) / 2;
    sunBtn.setBounds(rcWeatherPresets.getX() + 5, rcWeatherPresets.getY() + 60, nButtonWidth, nButtonWidth);
    windBtn.setBounds(rcWeatherPresets.getX() + 10 + nButtonWidth, rcWeatherPresets.getY() + 60, nButtonWidth, nButtonWidth);
    rainBtn.setBounds(rcWeatherPresets.getX() + 5, rcWeatherPresets.getY() + 65 + nButtonWidth, nButtonWidth, nButtonWidth);
    snowBtn.setBounds(rcWeatherPresets.getX() + 10 + nButtonWidth, rcWeatherPresets.getY() + 65 + nButtonWidth, nButtonWidth, nButtonWidth);
}

/*======================================================================================*/
void WeatherPresets::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();

    g.setColour(Colours::darkseagreen);
    Rectangle<int> rcWeatherPresets = rect; // x, y, width, height
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
