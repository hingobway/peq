#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class Biquad
{
public:
	enum FilterType
	{
		TYPE_PEAK,
		TYPE_SHELF_LOW,
		TYPE_SHELF_HIGH
	};

private:
	float PI = juce::MathConstants<float>::pi;

	juce::AudioProcessor &audioProcessor;
	float SAMPLE_RATE;

	// CONTROLS
	FilterType type;
	float frequency;
	float gain_db;
	float q;

	float
			// DELAYS
			x2, // x[n-2]
			x1, // x[n-1]
			y2, // y[n-2]
			y1, // y[n-1]

			// COEFFICIENTS
			b0,
			b1,
			b2,
			a0,
			a1,
			a2,

			// FACTORS
			b0a0,
			b1a0,
			b2a0,
			a1a0,
			a2a0;

public:
	Biquad(juce::AudioProcessor &p) : audioProcessor(p), x2(), x1(), y2(), y1(), b0(), b1(), b2(), a0(), a1(), a2(), b0a0(), b1a0(), b2a0(), a1a0(), a2a0()
	{
		SAMPLE_RATE = (float)audioProcessor.getSampleRate();
	}

	void set(FilterType type_, float freq_, float gain_db_, float q_)
	{

		// update member vars if new
		const float epsilon = 1e-6f;
		if (type == type_ && fabs(frequency - freq_) < epsilon && fabs(gain_db - gain_db_) < epsilon && fabs(q - q_) < epsilon)
			return;

		type = type_;
		frequency = freq_;
		gain_db = gain_db_;
		q = q_;

		float A = powf(10, gain_db / 40.0f);
		float sqA = sqrtf(A);
		float w0 = 2 * PI * frequency / SAMPLE_RATE;
		float csw = cosf(w0);
		float snw = sinf(w0);

		float alpha = 0.0;
		switch (type)
		{
		case TYPE_PEAK:
			alpha = snw / (2.0f * q);

			b0 = 1.0f + alpha * A;
			b1 = -2.f * csw;
			b2 = 1.0f - alpha * A;
			a0 = 1.0f + alpha / A;
			a1 = -2.0f * csw;
			a2 = 1.0f - alpha / A;

			break;
		case TYPE_SHELF_LOW:
			alpha = snw / 2.0f * sqrtf((A + 1.0f / A) * (1.0f / q - 1) + 2);

			b0 = A * ((A + 1) - (A - 1) * csw + 2.0f * sqA * alpha);
			b1 = 2.0f * A * ((A - 1) - (A + 1) * csw);
			b2 = A * ((A + 1) - (A - 1) * csw - 2.0f * sqA * alpha);
			a0 = (A + 1) + (A - 1) * csw + 2 * sqA * alpha;
			a1 = -2.0f * ((A - 1) + (A + 1) * csw);
			a2 = (A + 1) + (A - 1) * csw - 2.0f * sqA * alpha;

			break;
		case TYPE_SHELF_HIGH:
			alpha = snw / 2.0f * sqrtf((A + 1.0f / A) * (1.0f / q - 1) + 2);

			b0 = A * ((A + 1) + (A - 1) * csw + 2.0f * sqA * alpha);
			b1 = -2.0f * A * ((A - 1) + (A + 1) * csw);
			b2 = A * ((A + 1) + (A - 1) * csw - 2.0f * sqA * alpha);
			a0 = (A + 1) - (A - 1) * csw + 2 * sqA * alpha;
			a1 = 2.0f * ((A - 1) - (A + 1) * csw);
			a2 = (A + 1) - (A - 1) * csw - 2.0f * sqA * alpha;

			break;
		}

		b0a0 = b0 / a0;
		b1a0 = b1 / a0;
		b2a0 = b2 / a0;
		a1a0 = a1 / a0;
		a2a0 = a2 / a0;
	}

	float run(float x)
	{
		float y = b0a0 * x + b1a0 * x1 + b2a0 * x2 - a1a0 * y1 - a2a0 * y2;

		y = (y <= 1.0) ? y : 1.0;
		y = (y >= -1.0) ? y : -1.0;

		y2 = y1;
		x2 = x1;
		y1 = y;
		x1 = x;

		return y;
	}
};
