#include "DistanceGraphic.h"
#include "MainComponent.h"

#define TIME_DAY    0
#define TIME_NIGHT  1

/*======================================================================================*/
DistanceGraphic::DistanceGraphic(MainComponent& parentComponent)
    : m_pcMainComponent(parentComponent)
/*======================================================================================*/
{
    m_cDistanceSlider.setSliderStyle(Slider::LinearHorizontal);
    m_cDistanceSlider.setRange(1, 300);
    m_cDistanceSlider.setSkewFactorFromMidPoint(50.0);
    m_cDistanceSlider.setValue(100);
    m_cDistanceSlider.setTextBoxStyle(Slider::NoTextBox, false, 20 , 40);
    m_cDistanceSlider.setLookAndFeel(&m_lfStickmanLookAndFeel);
    m_cDistanceSlider.setMouseCursor(MouseCursor::NormalCursor);
    addAndMakeVisible(&m_cDistanceSlider);

    m_cDistanceText.setMultiLine(false);
    m_cDistanceText.setReadOnly(false);  
    m_cDistanceText.setScrollbarsShown(false);
    m_cDistanceText.setCaretVisible(false);
    m_cDistanceText.setColour(TextEditor::textColourId, Colours::black);
    m_cDistanceText.setColour(TextEditor::backgroundColourId, Colours::white);
    m_cDistanceText.setColour(TextEditor::outlineColourId, Colours::black);
    m_cDistanceText.setText(String(m_cDistanceSlider.getValue()));
    m_cDistanceText.setOpaque(true);
    addAndMakeVisible(&m_cDistanceText);

    m_cDistanceSlider.onValueChange = [this] 
    { 
        m_cDistanceText.setText("Distance: " + String((std::round(m_cDistanceSlider.getValue() * 10)/10)) + "m");
        m_pcMainComponent.vSetParameter(PARAMETER_DISTANCE, m_cDistanceSlider.getValue(), true);
    };

    // Load in image assets
    m_iSunnyImage = ImageFileFormat::loadFrom(BinaryData::sunnyScene_png, BinaryData::sunnyScene_pngSize);
    m_iWindyImage = ImageFileFormat::loadFrom(BinaryData::windyScene_png, BinaryData::windyScene_pngSize);
    m_iRainyImage = ImageFileFormat::loadFrom(BinaryData::rainyScene_png, BinaryData::rainyScene_pngSize);
    m_iSnowyImage = ImageFileFormat::loadFrom(BinaryData::snowyScene_png, BinaryData::snowyScene_pngSize);

    // Stage images
    m_iStageNormal = ImageFileFormat::loadFrom(BinaryData::overlay_stage_standard_png, BinaryData::overlay_stage_standard_pngSize);
    m_iStageRainy = ImageFileFormat::loadFrom(BinaryData::overlay_stage_rainy_png, BinaryData::overlay_stage_rainy_pngSize);
    m_iStageSnowy = ImageFileFormat::loadFrom(BinaryData::overlay_stage_snowy_png, BinaryData::overlay_stage_snowy_pngSize);
    m_iStageNormalNight = ImageFileFormat::loadFrom(BinaryData::overlay_stage_night_png, BinaryData::overlay_stage_night_pngSize);
    m_iStageRainyNight = ImageFileFormat::loadFrom(BinaryData::overlay_stage_rainy_night_png, BinaryData::overlay_stage_rainy_night_pngSize);

    // Wind overlays
    m_iWindLeft1 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_1_png, BinaryData::overlay_wind_left_1_pngSize);
    m_iWindLeft2 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_2_png, BinaryData::overlay_wind_left_2_pngSize);
    m_iWindLeft3 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_3_png, BinaryData::overlay_wind_left_3_pngSize);
    m_iWindLeft4 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_4_png, BinaryData::overlay_wind_left_4_pngSize);
    m_iWindLeft5 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_5_png, BinaryData::overlay_wind_left_5_pngSize);
    m_iWindRight1 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_1_png, BinaryData::overlay_wind_right_1_pngSize);
    m_iWindRight2 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_2_png, BinaryData::overlay_wind_right_2_pngSize);
    m_iWindRight3 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_3_png, BinaryData::overlay_wind_right_3_pngSize);
    m_iWindRight4 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_4_png, BinaryData::overlay_wind_right_4_pngSize);
    m_iWindRight5 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_5_png, BinaryData::overlay_wind_right_5_pngSize);

    // Cloud overlays
    m_iCloudsPartial = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_partial_png, BinaryData::overlay_clouds_partial_pngSize);
    m_iCloudsLight = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_light_png, BinaryData::overlay_clouds_light_pngSize);
    m_iCloudsDark = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_dark_png, BinaryData::overlay_clouds_dark_pngSize);
    m_iCloudsNight = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_night_png, BinaryData::overlay_clouds_night_pngSize);
    m_iCloudsPartialNight = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_partial_night_png, BinaryData::overlay_clouds_partial_night_pngSize);

    // Other
    m_iRainOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_precipitation_rain_png, BinaryData::overlay_precipitation_rain_pngSize);
    m_iSnowOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_precipitation_snow_png, BinaryData::overlay_precipitation_snow_pngSize);
    m_iSunOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_sun_png, BinaryData::overlay_sun_pngSize);
    m_iNightOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_night_png, BinaryData::overlay_night_pngSize);
}

/*======================================================================================*/
DistanceGraphic::~DistanceGraphic()
/*======================================================================================*/
{
    m_cDistanceSlider.setLookAndFeel(nullptr);
}


/*======================================================================================*/
void DistanceGraphic::resized()
/*======================================================================================*/
{
    Rectangle rcRect = getLocalBounds();
    Rectangle<int> rcDistanceGraph = rcRect;

    m_cDistanceSlider.setBounds(rcDistanceGraph.getX() + 100, rcDistanceGraph.getY() + 105, rcDistanceGraph.getWidth() - 120, 60);
    m_cDistanceText.setBounds(rcDistanceGraph.getX() + rcDistanceGraph.getWidth() - 120, rcDistanceGraph.getY() + 10, 110, 20);
}

/*======================================================================================*/
void DistanceGraphic::paint(Graphics& g)
/*======================================================================================*/
{
    // Get current parameters
    int nTemp = (int)m_pcMainComponent.dGetParameter(PARAMETER_TEMPERATURE);
    int nWindSpeed = (int)m_pcMainComponent.dGetParameter(PARAMETER_WIND_SPEED);
    bool bPrecipitation = (bool)m_pcMainComponent.dGetParameter(PARAMETER_PRECIPITATION);
    int nWindDirection = (int)m_pcMainComponent.dGetParameter(PARAMETER_WIND_DIRECTION);
    bool bCloudCover = (bool)m_pcMainComponent.dGetParameter(PARAMETER_CLOUD_COVER);
    int nTime = (int)m_pcMainComponent.dGetParameter(PARAMETER_TEMP_GRADIENT); // Treating temperature gradient as time of day

    // Set stickman
    if (nTemp <= 0)
    { m_cDistanceSlider.setLookAndFeel(&m_lfStickmanSnowyLookAndFeel); }
    else if (nTemp > 0 && bPrecipitation == ON)
    { m_cDistanceSlider.setLookAndFeel(&m_lfStickmanRainyLookAndFeel); }
    else
    { m_cDistanceSlider.setLookAndFeel(&m_lfStickmanLookAndFeel); }

    // Set the background fill
    Rectangle rcRect = getLocalBounds();
    if(bPrecipitation && nTemp > 0 && nTime == TIME_DAY) // Rainy day
    { g.setColour(Colours::slategrey); }
    else if (bPrecipitation == ON && nTemp > 0 && nTime == TIME_NIGHT) // Rainy night
    { g.setColour(Colours::darkgrey); }
    else if (bPrecipitation && nTemp <= 0 && nTime == TIME_DAY) // Snowy day
    { g.setColour(Colours::lightgrey); }
    else if (bPrecipitation && nTemp <= 0 && nTime == TIME_NIGHT) // Snowy night
    { g.setColour(Colours::dimgrey); }
    else if (!bPrecipitation && nTime == TIME_DAY) // Clear day
    { g.setColour(Colours::deepskyblue); }
    else if (!bPrecipitation && nTime == TIME_NIGHT) // Clear night
    { g.setColour(Colours::darkblue); }

    Rectangle<int> rcDistanceGraph = rcRect; 
    Rectangle<float> rcGraphicsBounds((float)rcDistanceGraph.getX(), (float)rcDistanceGraph.getY(), (float)rcDistanceGraph.getWidth(), (float)rcDistanceGraph.getHeight());
    g.fillRect(rcDistanceGraph);

    // Drawing stage
    if (nTemp <= 0) // Snowy
    { g.drawImage(m_iStageSnowy, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation && nTime == TIME_DAY) // Rainy day
    { g.drawImage(m_iStageRainy, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation && nTime == TIME_NIGHT) // Rainy night
    { g.drawImage(m_iStageRainyNight, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && !bPrecipitation && nTime == TIME_DAY) // Normal day
    { g.drawImage(m_iStageNormal, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && !bPrecipitation && nTime == TIME_NIGHT) // Normal night
    { g.drawImage(m_iStageNormalNight, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing sun
    if (!bPrecipitation && nTime == TIME_DAY)
    { g.drawImage(m_iSunOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (!bPrecipitation && nTime == TIME_NIGHT)
    { g.drawImage(m_iNightOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing wind
    if (nWindDirection == WIND_DIRECTION_UPWIND)
    {
        if (nWindSpeed == 0)
        { }
        else if (nWindSpeed < 20)
        { g.drawImage(m_iWindLeft1, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 40)
        { g.drawImage(m_iWindLeft2, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 60)
        { g.drawImage(m_iWindLeft3, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 80)
        { g.drawImage(m_iWindLeft4, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else
        { g.drawImage(m_iWindLeft5, rcGraphicsBounds, RectanglePlacement::centred, false); }
    }
    else if (nWindDirection == WIND_DIRECTION_DOWNWIND)
    {
        if (nWindSpeed == 0)
        { }
        else if (nWindSpeed < 20)
        { g.drawImage(m_iWindRight1, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 40)
        { g.drawImage(m_iWindRight2, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 60)
        { g.drawImage(m_iWindRight3, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 80)
        { g.drawImage(m_iWindRight4, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else
        { g.drawImage(m_iWindRight5, rcGraphicsBounds, RectanglePlacement::centred, false); }
    }

    // Drawing clouds
    if (nTemp <= 0 && bPrecipitation && nTime == TIME_DAY) // Snowy day
    { g.drawImage(m_iCloudsLight, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation && nTime == TIME_DAY) // Rainy day
    { g.drawImage(m_iCloudsDark, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (bPrecipitation && nTime == TIME_NIGHT) // Rainy/snowy night
    { g.drawImage(m_iCloudsNight, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (bCloudCover && nTime == TIME_DAY) // Partial clouds day
    { g.drawImage(m_iCloudsPartial, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (bCloudCover && nTime == TIME_NIGHT) // Partial clouds night
    { g.drawImage(m_iCloudsPartialNight, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing precipitation
    if (nTemp <= 0 && bPrecipitation)
    { g.drawImage(m_iSnowOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation)
    { g.drawImage(m_iRainOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }
}