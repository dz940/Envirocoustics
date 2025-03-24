#include "VolumeSlider.h"

/*======================================================================================*/
VolumeSlider::VolumeSlider()
/*======================================================================================*/
{
    setLookAndFeel(&m_lfVolumeSliderLookAndFeel);

    setSliderStyle(Slider::LinearVertical);
    setTextValueSuffix("dB");
    setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    setRange(-60.0, 10.0, 0.1); // dB scale (-60 dB to +10 dB)
    setSkewFactorFromMidPoint(-20.0); // Midpoint around -20 dB for natural feel
    setValue(0.0);
}

/*======================================================================================*/
VolumeSlider::~VolumeSlider()
/*======================================================================================*/
{
    setLookAndFeel(nullptr);
}

