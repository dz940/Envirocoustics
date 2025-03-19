#include "ConditionControls.h"
#include "MainComponent.h"

/*======================================================================================*/
ConditionControls::ConditionControls(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    // Initialise controls
    temperatureKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_TEMPERATURE, (int)temperatureKnob.getValue(), true); };
    temperatureKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    temperatureKnob.setTextValueSuffix("C");
    temperatureKnob.setRange(-10, 50, 1);
    temperatureKnob.setValue(20);
    temperatureKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    temperatureKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&temperatureKnob);

    windSpeedKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_WIND_SPEED, (int)windSpeedKnob.getValue(), true); };
    windSpeedKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    windSpeedKnob.setTextValueSuffix("km/h");
    windSpeedKnob.setRange(0, 100, 1);
    windSpeedKnob.setValue(10);
    windSpeedKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    windSpeedKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&windSpeedKnob);

    humidityKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_HUMIDITY, (int)humidityKnob.getValue(), false); };
    humidityKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    humidityKnob.setTextValueSuffix("%");
    humidityKnob.setRange(5, 100, 1);
    humidityKnob.setValue(50);
    humidityKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    humidityKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&humidityKnob);

    pressureKnob.onValueChange = [this] {mainComponent.vSetParameter(PARAMETER_PRESSURE, (int)pressureKnob.getValue(), false); };
    pressureKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    pressureKnob.setTextValueSuffix("mb");
    pressureKnob.setRange(850, 1150, 1);
    pressureKnob.setValue(1000);
    pressureKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    pressureKnob.setLookAndFeel(&rotaryLookAndFeel);
    addAndMakeVisible(&pressureKnob);

    temperatureLapseBtn.onClick = [this] { mainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, temperatureInversionBtn.getToggleState(), false); };
    temperatureLapseBtn.setButtonText("Lapse");
    temperatureLapseBtn.setClickingTogglesState(true);
    temperatureLapseBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    temperatureLapseBtn.setEnabled(true);
    temperatureLapseBtn.setRadioGroupId(1);
    temperatureLapseBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&temperatureLapseBtn);

    temperatureInversionBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, temperatureInversionBtn.getToggleState(), false); };
    temperatureInversionBtn.setButtonText("Inversion");
    temperatureInversionBtn.setClickingTogglesState(true);
    temperatureInversionBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    temperatureInversionBtn.setEnabled(true);
    temperatureInversionBtn.setRadioGroupId(1);
    addAndMakeVisible(&temperatureInversionBtn);

    precipitationOffBtn.onClick = [this] 
    {
            if (precipitationOffBtn.getToggleState() == ON)
            { 
                mainComponent.vSetParameter(PARAMETER_PRECIPITATION, !precipitationOffBtn.getToggleState(), true); 
                cloudOffBtn.setEnabled(true);
                humidityKnob.setValue(50);
                humidityKnob.setEnabled(true);
            }
    };
    precipitationOffBtn.setClickingTogglesState(true);
    precipitationOffBtn.setButtonText("OFF");
    precipitationOffBtn.setColour(TextButton::buttonOnColourId, Colours::red);
    precipitationOffBtn.setEnabled(true);
    precipitationOffBtn.setRadioGroupId(2);
    precipitationOffBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&precipitationOffBtn);

    precipitationOnBtn.onClick = [this] 
    {
        if (precipitationOnBtn.getToggleState() == ON)
        {
            mainComponent.vSetParameter(PARAMETER_PRECIPITATION, precipitationOnBtn.getToggleState(), false);
            mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, precipitationOnBtn.getToggleState(), true);
            cloudOnBtn.setToggleState(true, dontSendNotification);
            cloudOffBtn.setEnabled(false);
            humidityKnob.setValue(90);
            humidityKnob.setEnabled(false);
        }
    };
    precipitationOnBtn.setButtonText("ON");
    precipitationOnBtn.setClickingTogglesState(true);
    precipitationOnBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    precipitationOnBtn.setEnabled(true);
    precipitationOnBtn.setRadioGroupId(2);
    addAndMakeVisible(&precipitationOnBtn);

    windLeftBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, windRightBtn.getToggleState(), true); };
    windLeftBtn.setButtonText("Upwind");
    windLeftBtn.setClickingTogglesState(true);
    windLeftBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    windLeftBtn.setEnabled(true);
    windLeftBtn.setRadioGroupId(3);
    windLeftBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&windLeftBtn);

    windRightBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, windRightBtn.getToggleState(), true); };
    windRightBtn.setButtonText("Downwind");
    windRightBtn.setClickingTogglesState(true);
    windRightBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    windRightBtn.setEnabled(true);
    windRightBtn.setRadioGroupId(3);
    addAndMakeVisible(&windRightBtn);

    cloudOffBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, cloudOnBtn.getToggleState(), true); };
    cloudOffBtn.setClickingTogglesState(true);
    cloudOffBtn.setButtonText("OFF");
    cloudOffBtn.setColour(TextButton::buttonOnColourId, Colours::red);
    cloudOffBtn.setEnabled(true);
    cloudOffBtn.setRadioGroupId(5);
    cloudOffBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&cloudOffBtn);

    cloudOnBtn.onClick = [this] {mainComponent.vSetParameter(PARAMETER_CLOUD_COVER, cloudOnBtn.getToggleState(), true); };
    cloudOnBtn.setButtonText("ON");
    cloudOnBtn.setClickingTogglesState(true);
    cloudOnBtn.setColour(TextButton::buttonOnColourId, Colours::green);
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
                precipitationOnBtn.setToggleState(false, dontSendNotification);
                precipitationOffBtn.setToggleState(true, dontSendNotification);
            }
            else if (nValue == ON)
            {
                precipitationOnBtn.setToggleState(true, dontSendNotification);
                precipitationOffBtn.setToggleState(false, dontSendNotification);
            }
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            if (nValue == WIND_DIRECTION_UPWIND)
            {
                windLeftBtn.setToggleState(true, dontSendNotification);
                windRightBtn.setToggleState(false, dontSendNotification);
            }
            else if (nValue == WIND_DIRECTION_DOWNWIND)
            {
                windLeftBtn.setToggleState(false, dontSendNotification);
                windRightBtn.setToggleState(true, dontSendNotification);
            }
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            if (nValue == TEMPERATURE_LAPSE)
            {
                temperatureLapseBtn.setToggleState(true, dontSendNotification);
                temperatureInversionBtn.setToggleState(false, dontSendNotification);
            }
            else if (nValue == TEMPERATURE_INVERSION)
            {
                temperatureLapseBtn.setToggleState(false, dontSendNotification);
                temperatureInversionBtn.setToggleState(true, dontSendNotification);
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
                cloudOnBtn.setToggleState(false, dontSendNotification);
                cloudOffBtn.setToggleState(true, dontSendNotification);
            }
            else if (nValue == ON)
            {
                cloudOnBtn.setToggleState(true, dontSendNotification);
                cloudOffBtn.setToggleState(false, dontSendNotification);
            }
            break;
        }
    }
}

/*======================================================================================*/
void ConditionControls::resized()
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();
    Rectangle<int> rcConditionControls = rect;
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
void ConditionControls::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();

    // Condition controls
    g.setColour(Colours::darkseagreen);
    Rectangle<int> rcConditionControls = rect; // x, y, width, height
    g.fillRect(rcConditionControls); // Fill the rectangle
    g.setFont(20.0f);
    g.setColour(Colours::black);
    Rectangle<int> rcConditionControlsTitle(rcConditionControls.getX(),
                            rcConditionControls.getY(),
                            rcConditionControls.getWidth(),
                            30);
    g.drawText("Weather controls", rcConditionControlsTitle, Justification::centred, true);

    int nPanelWidths = (rcConditionControls.getWidth() - 20) / 4;
    g.setFont(15.0f);
    Rectangle<int> rcTemperatureText(rcConditionControls.getX() + rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 35,
                            nPanelWidths,
                            20);
    g.drawText("Temperature", rcTemperatureText, Justification::centred, true);

    Rectangle<int> rcGradientText(rcConditionControls.getX() + rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 110,
                            nPanelWidths,
                            20);
    g.drawText("Gradient", rcGradientText, Justification::centred, true);

    Rectangle<int> rcWindControlsTitle(rcConditionControls.getX() + 3 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 35,
                            nPanelWidths,
                            20);
    g.drawText("Wind Speed", rcWindControlsTitle, Justification::centred, true);

    Rectangle<int> rcWindDirectionText(rcConditionControls.getX() + 3 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
                            rcConditionControls.getY() + 110,
                            nPanelWidths,
                            20);
    g.drawText("Direction", rcWindDirectionText, Justification::centred, true);

    Rectangle<int> rcHumidityControlsTitle(rcConditionControls.getX() + 5 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Humidity", rcHumidityControlsTitle, Justification::centred, true);

    Rectangle<int> rcPrecipitationText(rcConditionControls.getX() + 5 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Precipitation", rcPrecipitationText, Justification::centred, true);

    Rectangle<int> rcPressureControlsTitle(rcConditionControls.getX() + 7 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Pressure", rcPressureControlsTitle, Justification::centred, true);

    Rectangle<int> rcCloudCoverText(rcConditionControls.getX() + 7 * rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Clouds", rcCloudCoverText, Justification::centred, true);
}