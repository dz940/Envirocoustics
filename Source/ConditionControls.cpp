#include "ConditionControls.h"
#include "MainComponent.h"

/*======================================================================================*/
ConditionControls::ConditionControls(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    // Initialise controls
    temperatureKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, (int)temperatureKnob.getValue(), true); };
    temperatureKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    temperatureKnob.setTextValueSuffix("C");
    temperatureKnob.setRange(-10, 50, 1);
    temperatureKnob.setValue(20);
    temperatureKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 55, 20);
    temperatureKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&temperatureKnob);

    windSpeedKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_WIND_SPEED, (int)windSpeedKnob.getValue(), true); };
    windSpeedKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    windSpeedKnob.setTextValueSuffix("km/h");
    windSpeedKnob.setRange(0, 100, 1);
    windSpeedKnob.setValue(10);
    windSpeedKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 55, 20);
    windSpeedKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&windSpeedKnob);

    humidityKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_HUMIDITY, (int)humidityKnob.getValue(), false); };
    humidityKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    humidityKnob.setTextValueSuffix("%");
    humidityKnob.setRange(0, 100, 1);
    humidityKnob.setValue(50);
    humidityKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 55, 20);
    humidityKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&humidityKnob);

    pressureKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_PRESSURE, (int)pressureKnob.getValue(), false); };
    pressureKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pressureKnob.setTextValueSuffix("mb");
    pressureKnob.setRange(850, 1150, 1);
    pressureKnob.setValue(1000);
    pressureKnob.setTextBoxStyle(juce::Slider::TextBoxRight, false, 55, 20);
    pressureKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&pressureKnob);

    temperatureLapseBtn.onClick = [this] { mainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, temperatureInversionBtn.getToggleState(), false); };
    temperatureLapseBtn.setButtonText("Lapse");
    temperatureLapseBtn.setClickingTogglesState(true);
    temperatureLapseBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    temperatureLapseBtn.setEnabled(true);
    temperatureLapseBtn.setRadioGroupId(1);
    temperatureLapseBtn.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&temperatureLapseBtn);

    temperatureInversionBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, temperatureInversionBtn.getToggleState(), false); };
    temperatureInversionBtn.setButtonText("Inversion");
    temperatureInversionBtn.setClickingTogglesState(true);
    temperatureInversionBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    temperatureInversionBtn.setEnabled(true);
    temperatureInversionBtn.setRadioGroupId(1);
    addAndMakeVisible(&temperatureInversionBtn);

    precipitationOffBtn.onClick = [this] 
    {
            if (precipitationOffBtn.getToggleState() == ON)
            { 
                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, !precipitationOffBtn.getToggleState(), true); 
                cloudOffBtn.setEnabled(true);
            }
    };
    precipitationOffBtn.setClickingTogglesState(true);
    precipitationOffBtn.setButtonText("OFF");
    precipitationOffBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    precipitationOffBtn.setEnabled(true);
    precipitationOffBtn.setRadioGroupId(2);
    precipitationOffBtn.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&precipitationOffBtn);

    precipitationOnBtn.onClick = [this] 
    {
        if (precipitationOnBtn.getToggleState() == ON)
        {
            mainComponent.vSetParameter(PARAMETER_PRECIPITATION, precipitationOnBtn.getToggleState(), false);
            mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, precipitationOnBtn.getToggleState(), true);
            cloudOnBtn.setToggleState(true, juce::dontSendNotification);
            cloudOffBtn.setEnabled(false);
        }
    };
    precipitationOnBtn.setButtonText("ON");
    precipitationOnBtn.setClickingTogglesState(true);
    precipitationOnBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    precipitationOnBtn.setEnabled(true);
    precipitationOnBtn.setRadioGroupId(2);
    addAndMakeVisible(&precipitationOnBtn);

    windLeftBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, windRightBtn.getToggleState(), true); };
    windLeftBtn.setButtonText("Left");
    windLeftBtn.setClickingTogglesState(true);
    windLeftBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    windLeftBtn.setEnabled(true);
    windLeftBtn.setRadioGroupId(3);
    windLeftBtn.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&windLeftBtn);

    windRightBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, windRightBtn.getToggleState(), true); };
    windRightBtn.setButtonText("Right");
    windRightBtn.setClickingTogglesState(true);
    windRightBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    windRightBtn.setEnabled(true);
    windRightBtn.setRadioGroupId(3);
    addAndMakeVisible(&windRightBtn);

    cloudOffBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, cloudOnBtn.getToggleState(), true); };
    cloudOffBtn.setClickingTogglesState(true);
    cloudOffBtn.setButtonText("OFF");
    cloudOffBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    cloudOffBtn.setEnabled(true);
    cloudOffBtn.setRadioGroupId(5);
    cloudOffBtn.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(&cloudOffBtn);

    cloudOnBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, cloudOnBtn.getToggleState(), true); };
    cloudOnBtn.setButtonText("ON");
    cloudOnBtn.setClickingTogglesState(true);
    cloudOnBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    cloudOnBtn.setEnabled(true);
    cloudOnBtn.setRadioGroupId(5);
    addAndMakeVisible(&cloudOnBtn);
}

/*======================================================================================*/
ConditionControls::~ConditionControls()
/*======================================================================================*/
{
    // Release resources
    temperatureKnob.setLookAndFeel(nullptr);
    humidityKnob.setLookAndFeel(nullptr);
    windSpeedKnob.setLookAndFeel(nullptr);
    pressureKnob.setLookAndFeel(nullptr);
}

/*======================================================================================*/
void ConditionControls::vSetParameter(int nParameter, int nValue)
/*======================================================================================*/
{
    // Set parameters of the controls
    switch (nParameter)
    {
        case PARAMETER_TEMPERATURE:
        {
            temperatureKnob.setValue(nValue);
            break;
        }
        case PARAMETER_HUMIDITY:
        {
            humidityKnob.setValue(nValue);
            break;
        }
        case PARAMETER_WIND_SPEED:
        {
            windSpeedKnob.setValue(nValue);
            break;
        }
        case PARAMETER_PRECIPITATION:
        {
            if (nValue == OFF)
            {
                precipitationOnBtn.setToggleState(false, juce::dontSendNotification);
                precipitationOffBtn.setToggleState(true, juce::dontSendNotification);
            }
            else if (nValue == ON)
            {
                precipitationOnBtn.setToggleState(true, juce::dontSendNotification);
                precipitationOffBtn.setToggleState(false, juce::dontSendNotification);
            }
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            if (nValue == WIND_DIRECTION_LEFT)
            {
                windLeftBtn.setToggleState(true, juce::dontSendNotification);
                windRightBtn.setToggleState(false, juce::dontSendNotification);
            }
            else if (nValue == WIND_DIRECTION_RIGHT)
            {
                windLeftBtn.setToggleState(false, juce::dontSendNotification);
                windRightBtn.setToggleState(true, juce::dontSendNotification);
            }
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            if (nValue == TEMPERATURE_LAPSE)
            {
                temperatureLapseBtn.setToggleState(true, juce::dontSendNotification);
                temperatureInversionBtn.setToggleState(false, juce::dontSendNotification);
            }
            else if (nValue == TEMPERATURE_INVERSION)
            {
                temperatureLapseBtn.setToggleState(false, juce::dontSendNotification);
                temperatureInversionBtn.setToggleState(true, juce::dontSendNotification);
            }
            break;
        }
        case PARAMETER_PRESSURE:
        {
            pressureKnob.setValue(nValue);
            break;
        }
        case PARAMETER_CLOUD_COVER:
        {
            if (nValue == OFF)
            {
                cloudOnBtn.setToggleState(false, juce::dontSendNotification);
                cloudOffBtn.setToggleState(true, juce::dontSendNotification);
            }
            else if (nValue == ON)
            {
                cloudOnBtn.setToggleState(true, juce::dontSendNotification);
                cloudOffBtn.setToggleState(false, juce::dontSendNotification);
            }
            break;
        }
    }
}

/*======================================================================================*/
void ConditionControls::resized()
/*======================================================================================*/
{
    juce::Rectangle rect = getLocalBounds();
    juce::Rectangle<int> rcConditionControls = rect;
    int nControlWidth = rcConditionControls.getWidth() / 4;
    int nButtonBorderWidth = 5;

    temperatureKnob.setBounds(rcConditionControls.getX(), rcConditionControls.getY() + 60, nControlWidth, 35);
    windSpeedKnob.setBounds(rcConditionControls.getX() + nControlWidth, rcConditionControls.getY() + 60, nControlWidth, 35);
    humidityKnob.setBounds(rcConditionControls.getX() + nControlWidth * 2, rcConditionControls.getY() + 60, nControlWidth, 35);
    pressureKnob.setBounds(rcConditionControls.getX() + nControlWidth * 3, rcConditionControls.getY() + 60, nControlWidth, 35);

    temperatureLapseBtn.setBounds(rcConditionControls.getX() + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    temperatureInversionBtn.setBounds(rcConditionControls.getX() + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    windLeftBtn.setBounds(rcConditionControls.getX() + nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    windRightBtn.setBounds(rcConditionControls.getX() + nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    precipitationOnBtn.setBounds(rcConditionControls.getX() + 2 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    precipitationOffBtn.setBounds(rcConditionControls.getX() + 2 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    cloudOnBtn.setBounds(rcConditionControls.getX() + 3 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    cloudOffBtn.setBounds(rcConditionControls.getX() + 3 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);
}

/*======================================================================================*/
void ConditionControls::paint(juce::Graphics& g)
/*======================================================================================*/
{
    juce::Rectangle rect = getLocalBounds();

    // Condition controls
    g.setColour(juce::Colours::darkseagreen);
    juce::Rectangle<int> rcConditionControls = rect; // x, y, width, height
    g.fillRect(rcConditionControls); // Fill the rectangle
    g.setFont(20.0f);
    g.setColour(juce::Colours::black);
    juce::Rectangle<int> rcConditionControlsTitle(rcConditionControls.getX(),
                            rcConditionControls.getY(),
                            rcConditionControls.getWidth(),
                            30);
    g.drawText("Condition controls", rcConditionControlsTitle, juce::Justification::centred, true);

    int nPanelWidths = (rcConditionControls.getWidth() - 20) / 4;
    g.setFont(15.0f);
    juce::Rectangle<int> rcTemperatureText(rcConditionControls.getX() + rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 35,
                            nPanelWidths,
                            20);
    g.drawText("Temperature", rcTemperatureText, juce::Justification::centred, true);

    juce::Rectangle<int> rcGradientText(rcConditionControls.getX() + rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 110,
                            nPanelWidths,
                            20);
    g.drawText("Gradient", rcGradientText, juce::Justification::centred, true);

    juce::Rectangle<int> rcWindControlsTitle(rcConditionControls.getX() + 3 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 35,
                            nPanelWidths,
                            20);
    g.drawText("Wind Speed", rcWindControlsTitle, juce::Justification::centred, true);

    juce::Rectangle<int> rcWindDirectionText(rcConditionControls.getX() + 3 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 110,
                            nPanelWidths,
                            20);
    g.drawText("Direction", rcWindDirectionText, juce::Justification::centred, true);

    juce::Rectangle<int> rcHumidityControlsTitle(rcConditionControls.getX() + 5 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Humidity", rcHumidityControlsTitle, juce::Justification::centred, true);

    juce::Rectangle<int> rcPrecipitationText(rcConditionControls.getX() + 5 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Precipitation", rcPrecipitationText, juce::Justification::centred, true);

    juce::Rectangle<int> rcPressureControlsTitle(rcConditionControls.getX() + 7 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Pressure", rcPressureControlsTitle, juce::Justification::centred, true);

    juce::Rectangle<int> rcCloudCoverText(rcConditionControls.getX() + 7 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Clouds", rcCloudCoverText, juce::Justification::centred, true);
}