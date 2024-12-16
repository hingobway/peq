#pragma once

#include "../utilities.h"
#include "Knob.h"
#include "LAFs.h"

//==============================================================================
/*
 */
class Band : public juce::Component
{
public:
	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

	Band(juce::AudioProcessorValueTreeState &vts_, juce::String name_, bool shelf_) : vts(vts_), name(name_), shelf(shelf_)
	{
		std::stringstream gname{};
		gname << name << "_gain";
		knobGain.reset(new Knob(vts, gname.str(), false));
		std::stringstream fname{};
		fname << name << "_freq";
		knobFreq.reset(new Knob(vts, fname.str(), true));

		if (shelf)
		{
			buttonShelf.setLookAndFeel(&lafTextButton);
			buttonShelf.setButtonText("shelf");
			buttonShelf.setColour(juce::TextButton::buttonColourId, util::hexA(util::TW_ZINC_600, 0.8f));
			buttonShelf.setColour(juce::TextButton::buttonOnColourId, util::hexA(util::TW_SKY_600, 1));
			// buttonShelf.setToggleable(true);
			buttonShelf.setClickingTogglesState(true);

			std::stringstream sname{};
			sname << name << "_shelf";
			attShelf.reset(new ButtonAttachment(vts, sname.str(), buttonShelf));

			addAndMakeVisible(buttonShelf);
		}

		labelName.setText(name, juce::dontSendNotification);
		labelName.setFont(juce::FontOptions(18.0, juce::Font::bold));
		labelName.setColour(juce::Label::textColourId, util::hexA(util::TW_ZINC_100, 1));
		labelName.setJustificationType(juce::Justification::centred);

		addAndMakeVisible(labelName);
		addAndMakeVisible(knobGain.get());
		addAndMakeVisible(knobFreq.get());
	}

	void paint(juce::Graphics &g) override
	{
		juce::Path p;
		p.addRoundedRectangle(getLocalBounds().toFloat(), 8);
		g.setColour(util::hexA(util::TW_ZINC_800, 1));
		g.fillPath(p);
	}

	void resized() override
	{
		DBG("laying out band...");

		juce::FlexBox fb;
		juce::FlexBox fbKnobs;

		fb.flexDirection = juce::FlexBox::Direction::column;
		{
			fb.items.add(juce::FlexItem(labelName).withHeight(22));

			fbKnobs.flexDirection = juce::FlexBox::Direction::row;
			fbKnobs.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
			{
				fbKnobs.items.add(juce::FlexItem(*knobGain.get()).withFlex(1));
				fbKnobs.items.add(juce::FlexItem(*knobFreq.get()).withFlex(1));
			}
			fb.items.add(juce::FlexItem(fbKnobs).withFlex(1));
			fb.items.add(juce::FlexItem().withHeight(48));

			fb.items.add(juce::FlexItem(buttonShelf).withHeight(36));
		}

		fb.performLayout(getLocalBounds().reduced(10));
	}

private:
	juce::AudioProcessorValueTreeState &vts;

	juce::String name;
	bool shelf;

	juce::Rectangle<int> bg;

	// PARAMS

	juce::Label labelName;

	std::unique_ptr<Knob> knobGain;
	juce::Label labelGain;

	std::unique_ptr<Knob> knobFreq;
	juce::Label labelFreq;

	LAFTextButton lafTextButton;
	juce::TextButton buttonShelf;
	std::unique_ptr<ButtonAttachment> attShelf;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Band)
};

class BandWrapper : public juce::Component
{
public:
	BandWrapper(juce::AudioProcessorValueTreeState &vts)
	{
		//    band constructor(  , "band name" , with shelf  )
		bandLow.reset(new Band(vts, "low", true));
		bandMid.reset(new Band(vts, "mid", false));
		bandHigh.reset(new Band(vts, "high", true));
		addAndMakeVisible(bandLow.get());
		addAndMakeVisible(bandMid.get());
		addAndMakeVisible(bandHigh.get());
	}

	void paint(juce::Graphics &g) override
	{
		juce::Path p;
		p.addRoundedRectangle(getLocalBounds(), 8 + GAP);
		g.setColour(util::hexA(util::TW_ZINC_700, 1));
		g.fillPath(p);
	}

	void resized() override
	{

		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::row;
		{
			fb.items.add(juce::FlexItem(*bandLow.get()).withFlex(1));
			fb.items.add(juce::FlexItem().withWidth(GAP));
			fb.items.add(juce::FlexItem(*bandMid.get()).withFlex(1));
			fb.items.add(juce::FlexItem().withWidth(GAP));
			fb.items.add(juce::FlexItem(*bandHigh.get()).withFlex(1));
		}

		fb.performLayout(getLocalBounds().reduced(GAP));
	}

private:
	int GAP = 10;

	std::unique_ptr<Band> bandLow;
	std::unique_ptr<Band> bandMid;
	std::unique_ptr<Band> bandHigh;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandWrapper)
};
