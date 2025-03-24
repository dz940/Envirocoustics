#include "VolumeControl.h"
#include "MainComponent.h"

/*======================================================================================*/
VolumeControl::VolumeControl(MainComponent& parentComponent)
    : m_pcMainComponent(parentComponent)
/*======================================================================================*/
{
    m_cVolumeFader.setSliderStyle(Slider::LinearVertical);
    m_cVolumeFader.setTextValueSuffix("dB");
    m_cVolumeFader.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    m_cVolumeFader.setRange(-60.0, 10.0, 0.1); // dB scale (-60 dB to +10 dB)
    m_cVolumeFader.setValue(0.0);
    m_cVolumeFader.setLookAndFeel(&m_lfVolumeSliderLookAndFeel);
    addAndMakeVisible(m_cVolumeFader);
    addAndMakeVisible(m_cLevelMeter);
    m_cLevelMeter.setOpaque(true);
}

/*======================================================================================*/
VolumeControl::~VolumeControl()
/*======================================================================================*/
{
    m_cVolumeFader.setLookAndFeel(nullptr);
}

/*======================================================================================*/
double VolumeControl::dGetGain()
/*======================================================================================*/
{
    return m_cVolumeFader.getValue();
}

/*======================================================================================*/
void VolumeControl::resized()
/*======================================================================================*/
{

    Rectangle rect = getLocalBounds();
    Rectangle<int> rcVolumeControl = rect;

    m_cVolumeFader.setBounds(rcVolumeControl.getX() + 45, rcVolumeControl.getY() + 30, 60, rcVolumeControl.getHeight() - 40);
    m_cLevelMeter.setBounds(rcVolumeControl.getX() + rcVolumeControl.getWidth() - 40, m_cVolumeFader.getY() + 25, 6, 190);
}

/*======================================================================================*/
void VolumeControl::vSetMeterLevel(float fNewLevel)
/*======================================================================================*/
{
    m_cLevelMeter.vSetLevel(0.7f * m_cLevelMeter.fGetLevel() + 0.3f * jlimit(0.0f, 1.0f, fNewLevel));
    m_cLevelMeter.repaint();
}

/*======================================================================================*/
void VolumeControl::paint(Graphics& g)
/*======================================================================================*/
{
    g.setColour(Colours::darkseagreen);
    Rectangle<int> rcConditionControls = getLocalBounds();; // x, y, width, height
    g.fillRect(rcConditionControls); // Fill the rectangle
    g.setFont(20.0f);
    g.setColour(Colours::black);
    Rectangle<int> rcConditionControlsTitle(rcConditionControls.getX(),
        rcConditionControls.getY(),
        rcConditionControls.getWidth(),
        30);
    g.drawText("Volume", rcConditionControlsTitle, Justification::centred, true);
}