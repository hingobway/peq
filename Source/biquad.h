#pragma once

#include "JuceHeader.h"

class Biquad {
private:
	juce::AudioProcessor& audioProcessor;
	double SAMPLE_RATE;

	float x2; // x[n-2]
	float x1; // x[n-1]
	float y2; // y[n-2]
	float y1; // y[n-1]

	float b0;
	float b1;
	float b2;
	float a0;
	float a1;
	float a2;

public:
	float PI = juce::MathConstants<float>::pi;

	enum FilterType {
		TYPE_PEAK,
		TYPE_SHELF_LOW,
		TYPE_SHELF_HIGH
	};


	Biquad(juce::AudioProcessor& p) : audioProcessor(p), x2(), x1(), y2(), y1(), b0(), b1(), b2(), a0(), a1(), a2() {
		SAMPLE_RATE = audioProcessor.getSampleRate();
	}

	void set(FilterType t, float freq, float gain_db, float q) {
		float A = powf(10, gain_db / 40.0);
		float sqA = sqrtf(A);
		float w0 = 2 * PI * freq / SAMPLE_RATE;
		float csw = cosf(w0);
		float snw = sin(w0);

		float alpha = 0.0;
		switch (t) {
		case TYPE_PEAK:
			alpha = snw / (2.0 * q);

			b0 = 1.0 + alpha * A;
			b1 = -2.0 * csw;
			b2 = 1.0 - alpha * A;
			a0 = 1.0 + alpha / A;
			a1 = -2.0 * csw;
			a2 = 1.0 - alpha / A;

			break;
		case TYPE_SHELF_LOW:
			alpha = snw / 2.0 * sqrtf((A + 1.0 / A) * (1.0 / q - 1) + 2);

			b0 = A * ((A + 1) - (A - 1) * csw + 2.0 * sqA * alpha);
			b1 = 2.0 * A * ((A - 1) - (A + 1) * csw);
			b2 = A * ((A + 1) - (A - 1) * csw - 2.0 * sqA * alpha);
			a0 = (A + 1) + (A - 1) * csw + 2 * sqA * alpha;
			a1 = -2.0 * ((A - 1) + (A + 1) * csw);
			a2 = (A + 1) + (A - 1) * csw - 2.0 * sqA * alpha;

			break;
		case TYPE_SHELF_HIGH:
			alpha = snw / 2.0 * sqrtf((A + 1.0 / A) * (1.0 / q - 1) + 2);

			b0 = A * ((A + 1) + (A - 1) * csw + 2.0 * sqA * alpha);
			b1 = -2.0 * A * ((A - 1) + (A + 1) * csw);
			b2 = A * ((A + 1) + (A - 1) * csw - 2.0 * sqA * alpha);
			a0 = (A + 1) - (A - 1) * csw + 2 * sqA * alpha;
			a1 = 2.0 * ((A - 1) - (A + 1) * csw);
			a2 = (A + 1) - (A - 1) * csw - 2.0 * sqA * alpha;

			break;
		}
	}

	float run(float x) {
		float y = (b0 / a0) * x + (b1 / a0) * x1 + (b2 / a0) * x2 - (a1 / a0) * y1 - (a2 / a0) * y2;

		y = (y <= 1.0) ? y : 1.0;
		y = (y >= -1.0) ? y : -1.0;

		y2 = y1;
		x2 = x1;
		y1 = y;
		x1 = x;

		return y;
	}
};
