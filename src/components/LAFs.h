#pragma once

#include "JuceHeader.h"

class LAFKnob : public juce::LookAndFeel_V4 {
public:

	LAFKnob() {
		svgKnob = juce::Drawable::createFromImageData(BinaryData::knob_svg, BinaryData::knob_svgSize);
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
	{
		float PI = juce::MathConstants<float>::pi;

		juce::Rectangle<int> bounds(x, y, width, height);
		int bcx = bounds.getCentreX();
		int bcy = bounds.getCentreY();

		int iw = svgKnob->getWidth();
		int ih = svgKnob->getHeight();
		int cx = svgKnob->getBounds().getCentreX();
		int cy = svgKnob->getBounds().getCentreY();

		float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		svgKnob->setTransform(juce::AffineTransform::rotation(angle, cx, cy));
		svgKnob->drawAt(g, bcx - cx, bcy - cy, 1.0);
	}

private:
	std::unique_ptr<juce::Drawable> svgKnob;

};

class LAFTextButton : public juce::LookAndFeel_V4 {
public:
	LAFTextButton() {
	}

	void drawButtonBackground(juce::Graphics& g,
		juce::Button& button,
		const juce::Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown) override
	{
		auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
		auto cornerSize = bounds.getHeight() / 2.0;

		auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
			.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

		if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
			baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

		g.setColour(baseColour);
		juce::Path path;
		path.addRoundedRectangle(bounds.getX(), bounds.getY(),
			bounds.getWidth(), bounds.getHeight(),
			cornerSize);

		g.fillPath(path);
	}


private:
};
