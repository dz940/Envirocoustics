#include "ConditionControls.h"
#include "MainComponent.h"
#include "DrawingFunctions.h"

/*======================================================================================*/
ConditionControls::ConditionControls(MainComponent& parentComponent)
    : m_cMainComponent(parentComponent)
/*======================================================================================*/
{
    // Initialise controls
    m_cTemperatureKnob.onValueChange = [this] {m_cMainComponent.vSetParameter(PARAMETER_TEMPERATURE, (int)m_cTemperatureKnob.getValue(), true); };
    m_cTemperatureKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    m_cTemperatureKnob.setTextValueSuffix("C");
    m_cTemperatureKnob.setRange(-10, 50, 1);
    m_cTemperatureKnob.setValue(20);
    m_cTemperatureKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    m_cTemperatureKnob.setLookAndFeel(&m_lfRotaryLookAndFeel);
    addAndMakeVisible(&m_cTemperatureKnob);

    m_cWindSpeedKnob.onValueChange = [this] {m_cMainComponent.vSetParameter(PARAMETER_WIND_SPEED, (int)m_cWindSpeedKnob.getValue(), true); };
    m_cWindSpeedKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    m_cWindSpeedKnob.setTextValueSuffix("km/h");
    m_cWindSpeedKnob.setRange(0, 100, 1);
    m_cWindSpeedKnob.setValue(10);
    m_cWindSpeedKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    m_cWindSpeedKnob.setLookAndFeel(&m_lfRotaryLookAndFeel);
    addAndMakeVisible(&m_cWindSpeedKnob);

    m_cHumidityKnob.onValueChange = [this] {m_cMainComponent.vSetParameter(PARAMETER_HUMIDITY, (int)m_cHumidityKnob.getValue(), false); };
    m_cHumidityKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    m_cHumidityKnob.setTextValueSuffix("%");
    m_cHumidityKnob.setRange(5, 100, 1);
    m_cHumidityKnob.setValue(50);
    m_cHumidityKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    m_cHumidityKnob.setLookAndFeel(&m_lfRotaryLookAndFeel);
    addAndMakeVisible(&m_cHumidityKnob);

    m_cPressureKnob.onValueChange = [this] {m_cMainComponent.vSetParameter(PARAMETER_PRESSURE, (int)m_cPressureKnob.getValue(), false); };
    m_cPressureKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    m_cPressureKnob.setTextValueSuffix("mb");
    m_cPressureKnob.setRange(850, 1150, 1);
    m_cPressureKnob.setValue(1000);
    m_cPressureKnob.setTextBoxStyle(Slider::TextBoxRight, false, 55, 20);
    m_cPressureKnob.setLookAndFeel(&m_lfRotaryLookAndFeel);
    addAndMakeVisible(&m_cPressureKnob);

    m_cTemperatureLapseBtn.onClick = [this] {
        if (m_cTemperatureLapseBtn.getToggleState() == ON)
        { m_cMainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, TEMPERATURE_LAPSE, true); }};
    m_cTemperatureLapseBtn.setButtonText("Day"); // Temperature lapse is represented in the app as 'Day'
    m_cTemperatureLapseBtn.setClickingTogglesState(true);
    m_cTemperatureLapseBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cTemperatureLapseBtn.setEnabled(true);
    m_cTemperatureLapseBtn.setRadioGroupId(1);
    m_cTemperatureLapseBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&m_cTemperatureLapseBtn);

    m_cTemperatureInversionBtn.onClick = [this] {
        if (m_cTemperatureInversionBtn.getToggleState() == ON)
        { m_cMainComponent.vSetParameter(PARAMETER_TEMP_GRADIENT, TEMPERATURE_INVERSION, true); }};
    m_cTemperatureInversionBtn.setButtonText("Night"); // Temperature lapse is represented in the app as 'Night'
    m_cTemperatureInversionBtn.setClickingTogglesState(true);
    m_cTemperatureInversionBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cTemperatureInversionBtn.setEnabled(true);
    m_cTemperatureInversionBtn.setRadioGroupId(1);
    addAndMakeVisible(&m_cTemperatureInversionBtn);

    m_cPrecipitationOffBtn.onClick = [this] 
    {
            if (m_cPrecipitationOffBtn.getToggleState() == ON)
            { 
                m_cMainComponent.vSetParameter(PARAMETER_PRECIPITATION, !m_cPrecipitationOffBtn.getToggleState(), true);
                m_cCloudOffBtn.setEnabled(true);
                m_cHumidityKnob.setValue(50);
                m_cHumidityKnob.setEnabled(true);
            }
    };
    m_cPrecipitationOffBtn.setClickingTogglesState(true);
    m_cPrecipitationOffBtn.setButtonText("OFF");
    m_cPrecipitationOffBtn.setColour(TextButton::buttonOnColourId, Colours::red);
    m_cPrecipitationOffBtn.setEnabled(true);
    m_cPrecipitationOffBtn.setRadioGroupId(2);
    m_cPrecipitationOffBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&m_cPrecipitationOffBtn);

    m_cPrecipitationOnBtn.onClick = [this] 
    {
        if (m_cPrecipitationOnBtn.getToggleState() == ON)
        {
            m_cMainComponent.vSetParameter(PARAMETER_PRECIPITATION, m_cPrecipitationOnBtn.getToggleState(), false);
            m_cMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, m_cPrecipitationOnBtn.getToggleState(), true);
            m_cCloudOnBtn.setToggleState(true, dontSendNotification);
            m_cCloudOffBtn.setEnabled(false);
            m_cHumidityKnob.setValue(90);
            m_cHumidityKnob.setEnabled(false);
        }
    };
    m_cPrecipitationOnBtn.setButtonText("ON");
    m_cPrecipitationOnBtn.setClickingTogglesState(true);
    m_cPrecipitationOnBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cPrecipitationOnBtn.setEnabled(true);
    m_cPrecipitationOnBtn.setRadioGroupId(2);
    addAndMakeVisible(&m_cPrecipitationOnBtn);

    m_cWindLeftBtn.onClick = [this] {m_cMainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, m_cWindRightBtn.getToggleState(), true); };
    m_cWindLeftBtn.setButtonText("Upwind");
    m_cWindLeftBtn.setClickingTogglesState(true);
    m_cWindLeftBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cWindLeftBtn.setEnabled(true);
    m_cWindLeftBtn.setRadioGroupId(3);
    m_cWindLeftBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&m_cWindLeftBtn);

    m_cWindRightBtn.onClick = [this] {m_cMainComponent.vSetParameter(PARAMETER_WIND_DIRECTION, m_cWindRightBtn.getToggleState(), true); };
    m_cWindRightBtn.setButtonText("Downwind");
    m_cWindRightBtn.setClickingTogglesState(true);
    m_cWindRightBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cWindRightBtn.setEnabled(true);
    m_cWindRightBtn.setRadioGroupId(3);
    addAndMakeVisible(&m_cWindRightBtn);

    m_cCloudOffBtn.onClick = [this] {m_cMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, m_cCloudOnBtn.getToggleState(), true); };
    m_cCloudOffBtn.setClickingTogglesState(true);
    m_cCloudOffBtn.setButtonText("OFF");
    m_cCloudOffBtn.setColour(TextButton::buttonOnColourId, Colours::red);
    m_cCloudOffBtn.setEnabled(true);
    m_cCloudOffBtn.setRadioGroupId(5);
    m_cCloudOffBtn.setToggleState(true, dontSendNotification);
    addAndMakeVisible(&m_cCloudOffBtn);

    m_cCloudOnBtn.onClick = [this] {m_cMainComponent.vSetParameter(PARAMETER_CLOUD_COVER, m_cCloudOnBtn.getToggleState(), true); };
    m_cCloudOnBtn.setButtonText("ON");
    m_cCloudOnBtn.setClickingTogglesState(true);
    m_cCloudOnBtn.setColour(TextButton::buttonOnColourId, Colours::green);
    m_cCloudOnBtn.setEnabled(true);
    m_cCloudOnBtn.setRadioGroupId(5);
    addAndMakeVisible(&m_cCloudOnBtn);
}

/*======================================================================================*/
ConditionControls::~ConditionControls()
/*======================================================================================*/
{
    // Release resources
    m_cTemperatureKnob.setLookAndFeel(nullptr);
    m_cHumidityKnob.setLookAndFeel(nullptr);
    m_cWindSpeedKnob.setLookAndFeel(nullptr);
    m_cPressureKnob.setLookAndFeel(nullptr);
}

/*======================================================================================*/
void ConditionControls::vSetParameter(const int nParameter, const double dValue)
/*======================================================================================*/
{
    // Set parameters of the controls
    switch (nParameter)
    {
        case PARAMETER_TEMPERATURE:
        {
            m_cTemperatureKnob.setValue(dValue);
            break;
        }
        case PARAMETER_HUMIDITY:
        {
            m_cHumidityKnob.setValue(dValue);
            break;
        }
        case PARAMETER_WIND_SPEED:
        {
            m_cWindSpeedKnob.setValue(dValue);
            break;
        }
        case PARAMETER_PRECIPITATION:
        {
            if ((bool)dValue == OFF)
            {
                m_cPrecipitationOnBtn.setToggleState(false, dontSendNotification);
                m_cPrecipitationOffBtn.setToggleState(true, dontSendNotification);
            }
            else if ((bool)dValue == ON)
            {
                m_cPrecipitationOnBtn.setToggleState(true, dontSendNotification);
                m_cPrecipitationOffBtn.setToggleState(false, dontSendNotification);
            }
            break;
        }
        case PARAMETER_WIND_DIRECTION:
        {
            if ((bool)dValue == WIND_DIRECTION_UPWIND)
            {
                m_cWindLeftBtn.setToggleState(true, dontSendNotification);
                m_cWindRightBtn.setToggleState(false, dontSendNotification);
            }
            else if ((bool)dValue == WIND_DIRECTION_DOWNWIND)
            {
                m_cWindLeftBtn.setToggleState(false, dontSendNotification);
                m_cWindRightBtn.setToggleState(true, dontSendNotification);
            }
            break;
        }
        case PARAMETER_TEMP_GRADIENT:
        {
            if ((bool)dValue == TEMPERATURE_LAPSE)
            {
                m_cTemperatureLapseBtn.setToggleState(true, dontSendNotification);
                m_cTemperatureInversionBtn.setToggleState(false, dontSendNotification);
            }
            else if ((bool)dValue == TEMPERATURE_INVERSION)
            {
                m_cTemperatureLapseBtn.setToggleState(false, dontSendNotification);
                m_cTemperatureInversionBtn.setToggleState(true, dontSendNotification);
            }
            break;
        }
        case PARAMETER_PRESSURE:
        {
            m_cPressureKnob.setValue(dValue);
            break;
        }
        case PARAMETER_CLOUD_COVER:
        {
            if ((bool)dValue == OFF)
            {
                m_cCloudOnBtn.setToggleState(false, dontSendNotification);
                m_cCloudOffBtn.setToggleState(true, dontSendNotification);
            }
            else if ((bool)dValue == ON)
            {
                m_cCloudOnBtn.setToggleState(true, dontSendNotification);
                m_cCloudOffBtn.setToggleState(false, dontSendNotification);
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

    m_cTemperatureKnob.setBounds(rcConditionControls.getX(), rcConditionControls.getY() + 60, nControlWidth, 35);
    m_cWindSpeedKnob.setBounds(rcConditionControls.getX() + nControlWidth, rcConditionControls.getY() + 60, nControlWidth, 35);
    m_cHumidityKnob.setBounds(rcConditionControls.getX() + nControlWidth * 2, rcConditionControls.getY() + 60, nControlWidth, 35);
    m_cPressureKnob.setBounds(rcConditionControls.getX() + nControlWidth * 3, rcConditionControls.getY() + 60, nControlWidth, 35);

    m_cTemperatureLapseBtn.setBounds(rcConditionControls.getX() + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    m_cTemperatureInversionBtn.setBounds(rcConditionControls.getX() + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    m_cWindLeftBtn.setBounds(rcConditionControls.getX() + nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    m_cWindRightBtn.setBounds(rcConditionControls.getX() + nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    m_cPrecipitationOnBtn.setBounds(rcConditionControls.getX() + 2 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    m_cPrecipitationOffBtn.setBounds(rcConditionControls.getX() + 2 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);

    m_cCloudOnBtn.setBounds(rcConditionControls.getX() + 3 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 135, nControlWidth - 2 * nButtonBorderWidth, 20);
    m_cCloudOffBtn.setBounds(rcConditionControls.getX() + 3 * nControlWidth + nButtonBorderWidth, rcConditionControls.getY() + 160, nControlWidth - 2 * nButtonBorderWidth, 20);
}

/*======================================================================================*/
void ConditionControls::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();

    // Condition controls
    g.setColour(Colours::darkseagreen);
    Rectangle<float> rcConditionControls = rect.toFloat(); // x, y, width, height

    juce::Colour clBackGroundColour = juce::Colour(COLOUR_COMPONENT_BACKGROUND);
    juce::Colour clOutlineColour = juce::Colour(COLOUR_COMPONENT_OUTLINE);

    g.setColour(clBackGroundColour);
    g.fillRect(rcConditionControls);
    g.setColour(clOutlineColour);
    g.drawRect(rcConditionControls, 2);

    g.setFont(20.0f);
    g.setColour(Colours::black);
    Rectangle<int> rcConditionControlsTitle((int)rcConditionControls.getX(),
        (int)rcConditionControls.getY(),
        (int)rcConditionControls.getWidth(),
        30);
    g.drawText("Weather controls", rcConditionControlsTitle, Justification::centred, true);

    int nPanelWidths = ((int)rcConditionControls.getWidth() - 20) / 4;
    g.setFont(15.0f);
    Rectangle<int> rcTemperatureText((int)rcConditionControls.getX() + (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Temperature", rcTemperatureText, Justification::centred, true);

    Rectangle<int> rcGradientText((int)rcConditionControls.getX() + (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Day/Night", rcGradientText, Justification::centred, true);

    Rectangle<int> rcWindControlsTitle((int)rcConditionControls.getX() + 3 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Wind Speed", rcWindControlsTitle, Justification::centred, true);

    Rectangle<int> rcWindDirectionText((int)rcConditionControls.getX() + 3 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Direction", rcWindDirectionText, Justification::centred, true);

    Rectangle<int> rcHumidityControlsTitle((int)rcConditionControls.getX() + 5 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Humidity", rcHumidityControlsTitle, Justification::centred, true);

    Rectangle<int> rcPrecipitationText((int)rcConditionControls.getX() + 5 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Precipitation", rcPrecipitationText, Justification::centred, true);

    Rectangle<int> rcPressureControlsTitle((int)rcConditionControls.getX() + 7 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 35,
        nPanelWidths,
        20);
    g.drawText("Pressure", rcPressureControlsTitle, Justification::centred, true);

    Rectangle<int> rcCloudCoverText((int)rcConditionControls.getX() + 7 * (int)rcConditionControls.getWidth() / 8 - nPanelWidths / 2,
        (int)rcConditionControls.getY() + 110,
        nPanelWidths,
        20);
    g.drawText("Clouds", rcCloudCoverText, Justification::centred, true);
}