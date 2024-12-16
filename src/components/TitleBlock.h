#pragma once

#include "../utilities.h"

//==============================================================================
/*
 */
class TitleBlock : public juce::Component
{
public:
	TitleBlock()
	{
		// labelName.setFont(juce::Font(18.0, juce::Font::bold));
		// labelName.setColour(juce::Label::textColourId, util::hexA(util::TW_ZINC_100, 1));

		pluginName.setText("/PEQ/", juce::dontSendNotification);
		pluginName.setFont(juce::FontOptions(22.0));
		pluginName.setColour(juce::Label::textColourId, util::hexA(util::TW_ZINC_100, 1));
		pluginName.setJustificationType(juce::Justification::centredLeft);
		manufacturerName.setText("[foster]", juce::dontSendNotification);
		manufacturerName.setFont(juce::FontOptions(12.0, juce::Font::bold));
		manufacturerName.setColour(juce::Label::textColourId, util::hexA(util::TW_ZINC_400, 1));
		manufacturerName.setJustificationType(juce::Justification::centredRight);

		addAndMakeVisible(pluginName);
		addAndMakeVisible(manufacturerName);
	}

	void paint(juce::Graphics &g) override
	{
		juce::Path p;
		p.addRoundedRectangle(getLocalBounds().toFloat(), 12);
		g.setColour(util::hexA(util::TW_ZINC_700, 1));
		g.fillPath(p);
	}

	void resized() override
	{
		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::row;
		fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
		{
			fb.items.add(juce::FlexItem(pluginName).withFlex(1));
			fb.items.add(juce::FlexItem(manufacturerName).withFlex(1));
		}

		auto inner = getLocalBounds().withTrimmedLeft(PX).withTrimmedRight(PX).withTrimmedTop(PY).withTrimmedBottom(PY);
		fb.performLayout(inner);
	}

private:
	int PX = 20;
	int PY = 12;

	juce::Label pluginName;
	juce::Label manufacturerName;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBlock)
};
