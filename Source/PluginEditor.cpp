/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "utilities.h"

//==============================================================================
FosterPEQAudioProcessorEditor::FosterPEQAudioProcessorEditor(FosterPEQAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p), audioProcessor(p), vts(vts)
{

	addAndMakeVisible(titleBlock);

	bands.reset(new BandWrapper(vts));
	addAndMakeVisible(bands.get());

	setSize(700, 400);
	setResizable(true, false);
	setResizeLimits(665, 425, 2000, 1000);

}

FosterPEQAudioProcessorEditor::~FosterPEQAudioProcessorEditor()
{
}

//==============================================================================
void FosterPEQAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(util::hexA(util::TW_ZINC_900, 1));
}

void FosterPEQAudioProcessorEditor::resized()
{

	juce::FlexBox fb;

	fb.flexDirection = juce::FlexBox::Direction::column;
	{
		fb.items.add(juce::FlexItem(titleBlock).withHeight(48));
		fb.items.add(juce::FlexItem().withHeight(GAP));
		
		fb.items.add(juce::FlexItem(*bands.get()).withFlex(1));
	}

	auto inner = getLocalBounds().reduced(GAP);
	fb.performLayout(inner);
}
