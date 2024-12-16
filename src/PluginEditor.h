/*
	==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

	==============================================================================
*/

#pragma once

#include "PluginProcessor.h"

#include "components/Band.h"
#include "components/TitleBlock.h"

//==============================================================================
/**
 */
class FosterPEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	FosterPEQAudioProcessorEditor(FosterPEQAudioProcessor &parent, juce::AudioProcessorValueTreeState &vts);
	~FosterPEQAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics &) override;
	void resized() override;

	typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	FosterPEQAudioProcessor &audioProcessor;

	juce::AudioProcessorValueTreeState &vts;

	int GAP = 12;

	TitleBlock titleBlock;

	std::unique_ptr<BandWrapper> bands;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FosterPEQAudioProcessorEditor)
};
