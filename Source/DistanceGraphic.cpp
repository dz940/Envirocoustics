#include "DistanceGraphic.h"
#include "MainComponent.h"

/*======================================================================================*/
DistanceGraphic::DistanceGraphic(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    distanceSlider.setSliderStyle(Slider::LinearHorizontal);
    distanceSlider.setRange(0, 1000, 10);
    distanceSlider.setValue(100);
    distanceSlider.setTextBoxStyle(Slider::NoTextBox, false, 20 , 40);
    distanceSlider.setLookAndFeel(&stickmanLookAndFeel);
    distanceSlider.setMouseCursor(juce::MouseCursor::NormalCursor);
    addAndMakeVisible(&distanceSlider);

    distanceText.setMultiLine(false);
    distanceText.setReadOnly(false);   
    distanceText.setScrollbarsShown(false);
    distanceText.setCaretVisible(false);
    distanceText.setColour(TextEditor::textColourId, Colours::black);
    distanceText.setColour(TextEditor::backgroundColourId, Colours::white);
    distanceText.setColour(TextEditor::outlineColourId, Colours::black);
    distanceText.setText(String(distanceSlider.getValue()));
    addAndMakeVisible(&distanceText);

    distanceSlider.onValueChange = [this] { distanceText.setText("Distance: " + String(distanceSlider.getValue()) + "m"); 
    mainComponent.vSetParameter(PARAMETER_DISTANCE, (int)distanceSlider.getValue(), true); };

    // Load in image assets
    sunnyImage = ImageFileFormat::loadFrom(BinaryData::sunnyScene_png, BinaryData::sunnyScene_pngSize);
    windyImage = ImageFileFormat::loadFrom(BinaryData::windyScene_png, BinaryData::windyScene_pngSize);
    rainyImage = ImageFileFormat::loadFrom(BinaryData::rainyScene_png, BinaryData::rainyScene_pngSize);
    snowyImage = ImageFileFormat::loadFrom(BinaryData::snowyScene_png, BinaryData::snowyScene_pngSize);

    // Stage images
    stageNormal = ImageFileFormat::loadFrom(BinaryData::overlay_stage_standard_png, BinaryData::overlay_stage_standard_pngSize);
    stageRainy = ImageFileFormat::loadFrom(BinaryData::overlay_stage_rainy_png, BinaryData::overlay_stage_rainy_pngSize);
    stageSnowy = ImageFileFormat::loadFrom(BinaryData::overlay_stage_snowy_png, BinaryData::overlay_stage_snowy_pngSize);

    // Wind overlays
    windLeft1 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_1_png, BinaryData::overlay_wind_left_1_pngSize);
    windLeft2 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_2_png, BinaryData::overlay_wind_left_2_pngSize);
    windLeft3 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_3_png, BinaryData::overlay_wind_left_3_pngSize);
    windLeft4 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_4_png, BinaryData::overlay_wind_left_4_pngSize);
    windLeft5 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_left_5_png, BinaryData::overlay_wind_left_5_pngSize);
    windRight1 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_1_png, BinaryData::overlay_wind_right_1_pngSize);
    windRight2 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_2_png, BinaryData::overlay_wind_right_2_pngSize);
    windRight3 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_3_png, BinaryData::overlay_wind_right_3_pngSize);
    windRight4 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_4_png, BinaryData::overlay_wind_right_4_pngSize);
    windRight5 = ImageFileFormat::loadFrom(BinaryData::overlay_wind_right_5_png, BinaryData::overlay_wind_right_5_pngSize);

    // Cloud overlays
    cloudsPartial = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_partial_png, BinaryData::overlay_clouds_partial_pngSize);
    cloudsLight = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_light_png, BinaryData::overlay_clouds_light_pngSize);
    cloudsDark = ImageFileFormat::loadFrom(BinaryData::overlay_clouds_dark_png, BinaryData::overlay_clouds_dark_pngSize);

    // Other
    rainOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_precipitation_rain_png, BinaryData::overlay_precipitation_rain_pngSize);
    snowOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_precipitation_snow_png, BinaryData::overlay_precipitation_snow_pngSize);
    sunOverlay = ImageFileFormat::loadFrom(BinaryData::overlay_sun_png, BinaryData::overlay_sun_pngSize);

}

/*======================================================================================*/
DistanceGraphic::~DistanceGraphic()
/*======================================================================================*/
{
    distanceSlider.setLookAndFeel(nullptr);
}


/*======================================================================================*/
void DistanceGraphic::resized()
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();
    Rectangle<int> rcDistanceGraph = rect;

    distanceSlider.setBounds(rcDistanceGraph.getX() + 100, rcDistanceGraph.getY() + 105, rcDistanceGraph.getWidth() - 120, 60);
    distanceText.setBounds(rcDistanceGraph.getX() + rcDistanceGraph.getWidth() - 120, rcDistanceGraph.getY() + 10, 110, 20);
}

/*======================================================================================*/
void DistanceGraphic::paint(Graphics& g)
/*======================================================================================*/
{
    DBG("Painting distance graphics...\n");
    // Get current parameters
    int nTemp = mainComponent.nGetParameter(PARAMETER_TEMPERATURE);
    int nWindSpeed = mainComponent.nGetParameter(PARAMETER_WIND_SPEED);
    bool bPrecipitation = mainComponent.nGetParameter(PARAMETER_PRECIPITATION);
    int nWindDirection = mainComponent.nGetParameter(PARAMETER_WIND_DIRECTION);
    bool bCloudCover = mainComponent.nGetParameter(PARAMETER_CLOUD_COVER);

    // Set stickman
    if (nTemp <= 0)
    { distanceSlider.setLookAndFeel(&stickmanSnowyLookAndFeel); }
    else if (nTemp > 0 && bPrecipitation == ON)
    { distanceSlider.setLookAndFeel(&stickmanRainyLookAndFeel); }
    else
    { distanceSlider.setLookAndFeel(&stickmanLookAndFeel); }

    // Set the background fill
    Rectangle rect = getLocalBounds();
    if(bPrecipitation == ON && nTemp > 0)
    { g.setColour(Colours::grey); }
    else if (bPrecipitation && nTemp <= 0)
    { g.setColour(Colours::lightgrey); }
    else
    { g.setColour(Colours::deepskyblue); }

    Rectangle<int> rcDistanceGraph = rect; // x, y, width, height
    Rectangle<float> rcGraphicsBounds((float)rcDistanceGraph.getX(), (float)rcDistanceGraph.getY(), (float)rcDistanceGraph.getWidth(), (float)rcDistanceGraph.getHeight());
    g.fillRect(rcDistanceGraph); // Fill the rectangle

    // Drawing stage
    if (nTemp <= 0)
    { g.drawImage(stageSnowy, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation == ON)
    { g.drawImage(stageRainy, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else
    { g.drawImage(stageNormal, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing sun
    if (bPrecipitation == OFF)
    { g.drawImage(sunOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing wind
    if (nWindDirection == WIND_DIRECTION_LEFT)
    {
        if (nWindSpeed == 0)
        { }
        else if (nWindSpeed < 20)
        { g.drawImage(windLeft1, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 40)
        { g.drawImage(windLeft2, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 60)
        { g.drawImage(windLeft3, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if(nWindSpeed < 80)
        { g.drawImage(windLeft4, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else
        { g.drawImage(windLeft5, rcGraphicsBounds, RectanglePlacement::centred, false); }
    }
    else if (nWindDirection == WIND_DIRECTION_RIGHT)
    {
        if (nWindSpeed == 0)
        { }
        else if (nWindSpeed < 20)
        { g.drawImage(windRight1, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 40)
        { g.drawImage(windRight2, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 60)
        { g.drawImage(windRight3, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else if (nWindSpeed < 80)
        { g.drawImage(windRight4, rcGraphicsBounds, RectanglePlacement::centred, false); }
        else
        { g.drawImage(windRight5, rcGraphicsBounds, RectanglePlacement::centred, false); }
    }

    // Drawing clouds
    if (nTemp <= 0 && bPrecipitation == ON)
    { g.drawImage(cloudsLight, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation == ON)
    { g.drawImage(cloudsDark, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (bCloudCover == ON)
    { g.drawImage(cloudsPartial, rcGraphicsBounds, RectanglePlacement::centred, false); }

    // Drawing precipitation
    if (nTemp <= 0 && bPrecipitation == ON)
    { g.drawImage(snowOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }
    else if (nTemp > 0 && bPrecipitation == ON)
    { g.drawImage(rainOverlay, rcGraphicsBounds, RectanglePlacement::centred, false); }
}