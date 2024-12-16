/*
	==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

	==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FosterPEQAudioProcessor::FosterPEQAudioProcessor()
		:
#ifndef JucePlugin_PreferredChannelConfigurations
			AudioProcessor(BusesProperties()
												 .withInput("Input", juce::AudioChannelSet::stereo(), true)
												 .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#endif
			params(*this, nullptr, juce::Identifier("FosterPEQ"),
						 {// PARAMETERS

							// low band params
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("low_gain", 1), "Low Gain", -20.0, 20.0, 0.0),
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("low_freq", 1), "Low Frequency", 10.0, 19000.0, 500.0),
							std::make_unique<juce::AudioParameterBool>(
									juce::ParameterID("low_shelf", 1), "Low Shelf", false),

							// mid band params
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("mid_gain", 1), "Mid Gain", -20.0, 20.0, 0.0),
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("mid_freq", 1), "Mid Frequency", 10.0, 19000.0, 2000.0),

							// high band params
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("high_gain", 1), "High Gain", -20.0, 20.0, 0.0),
							std::make_unique<juce::AudioParameterFloat>(
									juce::ParameterID("high_freq", 1), "High Frequency", 10.0, 19000.0, 5000.0),
							std::make_unique<juce::AudioParameterBool>(
									juce::ParameterID("high_shelf", 1), "High Shelf", false)})
{
	DBG("constructed");

	pLowGain = params.getRawParameterValue("low_gain");
	pLowFreq = params.getRawParameterValue("low_freq");
	pLowShelf = params.getRawParameterValue("low_shelf");
	pMidGain = params.getRawParameterValue("mid_gain");
	pMidFreq = params.getRawParameterValue("mid_freq");
	pHighGain = params.getRawParameterValue("high_gain");
	pHighFreq = params.getRawParameterValue("high_freq");
	pHighShelf = params.getRawParameterValue("high_shelf");
}

FosterPEQAudioProcessor::~FosterPEQAudioProcessor()
{
}

//==============================================================================
const juce::String FosterPEQAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool FosterPEQAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool FosterPEQAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool FosterPEQAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double FosterPEQAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int FosterPEQAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
						// so this should be at least 1, even if you're not really implementing programs.
}

int FosterPEQAudioProcessor::getCurrentProgram()
{
	return 0;
}

void FosterPEQAudioProcessor::setCurrentProgram(int index)
{
	juce::ignoreUnused(index);
}

const juce::String FosterPEQAudioProcessor::getProgramName(int index)
{
	juce::ignoreUnused(index);
	return {};
}

void FosterPEQAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
	juce::ignoreUnused(index, newName);
}

//==============================================================================
void FosterPEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	juce::ignoreUnused(sampleRate, samplesPerBlock);

	// Use this method as the place to do any pre-playback
	// initialisation that you need..

	for (int c = 0; c < 2; c++)
	{
		for (int b = 0; b < 3; b++)
		{
			filters[c][b].reset(new Biquad(*this));
		}
	}
}

void FosterPEQAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FosterPEQAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

		// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void FosterPEQAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
	juce::ignoreUnused(midiMessages);

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.

	float q = 0.707f;

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		filters[channel][0]->set((*pLowShelf > 0) ? Biquad::TYPE_SHELF_LOW : Biquad::TYPE_PEAK, *pLowFreq, *pLowGain, q);
		filters[channel][1]->set(Biquad::TYPE_PEAK, *pMidFreq, *pMidGain, q);
		filters[channel][2]->set((*pHighShelf > 0) ? Biquad::TYPE_SHELF_HIGH : Biquad::TYPE_PEAK, *pHighFreq, *pHighGain, q);
		float *channelData = buffer.getWritePointer(channel);
		for (int i = 0; i < buffer.getNumSamples(); i++)
		{
			float y = channelData[i];

			y = filters[channel][0]->run(y);
			y = filters[channel][1]->run(y);
			y = filters[channel][2]->run(y);

			channelData[i] = y;
		}
	}
}

//==============================================================================
bool FosterPEQAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *FosterPEQAudioProcessor::createEditor()
{
	return new FosterPEQAudioProcessorEditor(*this, params);
}

//==============================================================================
void FosterPEQAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	DBG("called get state info");

	juce::ValueTree state = params.copyState(); // copy current state

	// copy to XML...

	// convert to xml version of current state
	std::unique_ptr<juce::XmlElement> xml(state.createXml()); // pointer to xml object
	// copy that xml into provided memory block
	copyXmlToBinary(*xml, destData);
}

void FosterPEQAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	DBG("called set state info");

	// convert stored state to xml
	std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

	// ensure state is valid
	if (xml.get() == nullptr)
		return;
	if (!xml->hasTagName(params.state.getType()))
		return;

	// swap current value tree for the one in the memory block
	params.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
	return new FosterPEQAudioProcessor();
}
