/*
	==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

	==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "biquad.h"

//==============================================================================
/**
 */
class FosterPEQAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
		,
																public juce::AudioProcessorARAExtension
#endif
{
public:
	//==============================================================================
	FosterPEQAudioProcessor();
	~FosterPEQAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

	//==============================================================================
	juce::AudioProcessorEditor *createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String &newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock &destData) override;
	void setStateInformation(const void *data, int sizeInBytes) override;

	juce::AudioProcessorValueTreeState params;

private:
	// filters for 2 channels and 3 bands
	std::unique_ptr<Biquad> filters[2][3];

	// parameter pointers
	std::atomic<float> *pLowGain = nullptr;
	std::atomic<float> *pLowFreq = nullptr;
	std::atomic<float> *pLowShelf = nullptr;

	std::atomic<float> *pMidGain = nullptr;
	std::atomic<float> *pMidFreq = nullptr;

	std::atomic<float> *pHighGain = nullptr;
	std::atomic<float> *pHighFreq = nullptr;
	std::atomic<float> *pHighShelf = nullptr;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FosterPEQAudioProcessor)
};
