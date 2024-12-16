#pragma once

#include <JuceHeader.h>

#include "LAFs.h"

//==============================================================================
/*
*/
class Knob : public juce::Component
{
public:
	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Knob(juce::AudioProcessorValueTreeState& vts, juce::String param, bool skew) : vts(vts), param(param)
	{
		knob.setLookAndFeel(&lafKnob);
		knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
		knob.setSkewFactor(skew ? 0.01 : 1.0);
		knob.setRange(knob.getRange(), 1.0);
		addAndMakeVisible(knob);

		DBG("attempting to attach param " << param);

		attKnob.reset(new SliderAttachment(vts, param, knob));

	}

	void paint(juce::Graphics& g) override
	{
		//g.drawImageAt(imgKnob, 0, 0);
	}

	void resized() override
	{
		//knob.setBounds(0,0,imgKnob.getWidth(), imgKnob.getHeight());

		knob.setBounds(getLocalBounds().reduced(10));
	}

private:
	juce::AudioProcessorValueTreeState& vts;
	juce::String param;

	LAFKnob lafKnob;
	juce::Slider knob;
	std::unique_ptr<SliderAttachment> attKnob;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};
