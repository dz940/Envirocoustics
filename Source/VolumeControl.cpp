#include "VolumeControl.h"
#include "MainComponent.h"

/*======================================================================================*/
VolumeControl::VolumeControl(MainComponent& parentComponent)
    : mainComponent(parentComponent)
/*======================================================================================*/
{
    volumeFader.setSliderStyle(Slider::LinearVertical);
    volumeFader.setTextValueSuffix("dB");
    volumeFader.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    volumeFader.setRange(-60.0, 10.0, 0.1); // dB scale (-60 dB to +10 dB)
    volumeFader.setValue(0.0);
    volumeFader.setLookAndFeel(&volumeSliderLookAndFeel);
    addAndMakeVisible(volumeFader);
    addAndMakeVisible(levelMeter);
    levelMeter.setOpaque(true);
}

/*======================================================================================*/
VolumeControl::~VolumeControl()
/*======================================================================================*/
{
    volumeFader.setLookAndFeel(nullptr);
}

/*======================================================================================*/
double VolumeControl::getGain()
/*======================================================================================*/
{
    return volumeFader.getValue();
}

/*======================================================================================*/
void VolumeControl::resized()
/*======================================================================================*/
{

    Rectangle rect = getLocalBounds();
    Rectangle<int> rcVolumeControl = rect;

    volumeFader.setBounds(rcVolumeControl.getX() + 45, rcVolumeControl.getY() + 30, 60, rcVolumeControl.getHeight() - 40); 
    levelMeter.setBounds(rcVolumeControl.getX() + rcVolumeControl.getWidth() - 40, volumeFader.getY() + 25, 6, 190);
}

/*======================================================================================*/
void VolumeControl::setMeterLevel(float newLevel)
/*======================================================================================*/
{
    levelMeter.setLevel(0.7f * levelMeter.getLevel() + 0.3f * jlimit(0.0f, 1.0f, newLevel));
    levelMeter.repaint();
}

/*======================================================================================*/
void VolumeControl::paint(Graphics& g)
/*======================================================================================*/
{
    Rectangle rect = getLocalBounds();
    //g.fillAll(Colours::darkseagreen);
    g.setColour(Colours::darkseagreen);
    Rectangle<int> rcConditionControls = rect; // x, y, width, height
    g.fillRect(rcConditionControls); // Fill the rectangle
    g.setFont(20.0f);
    g.setColour(Colours::black);
    Rectangle<int> rcConditionControlsTitle(rcConditionControls.getX(),
        rcConditionControls.getY(),
        rcConditionControls.getWidth(),
        30);
    g.drawText("Volume", rcConditionControlsTitle, Justification::centred, true);
}