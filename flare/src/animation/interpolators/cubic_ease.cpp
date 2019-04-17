#include "flare/animation/interpolators/cubic_ease.hpp"
#include <cmath>

using namespace flare;

const int NewtonIterations = 4;
const float NewtonMinSlope = 0.001f;
const float SubdivisionPrecision = 0.0000001f;
const int SubdivisionMaxIterations = 10;

// Returns x(t) given t, x1, and x2, or y(t) given t, y1, and y2.
float calcBezier(float aT, float aA1, float aA2)
{
	return (((1.0 - 3.0 * aA2 + 3.0 * aA1) * aT + (3.0 * aA2 - 6.0 * aA1)) * aT + (3.0 * aA1)) * aT;
}

// Returns dx/dt given t, x1, and x2, or dy/dt given t, y1, and y2.
float getSlope(float aT, float aA1, float aA2)
{
	return 3.0 * (1.0 - 3.0 * aA2 + 3.0 * aA1) * aT * aT + 2.0 * (3.0 * aA2 - 6.0 * aA1) * aT + (3.0 * aA1);
}

CubicEase::CubicEase() : m_X1(0.0f), m_Y1(0.0f), m_X2(0.0f), m_Y2(0.0f) {}
CubicEase::CubicEase(float x1, float y1, float x2, float y2) { initialize(x1, y1, x2, y2); }

void CubicEase::initialize(float x1, float y1, float x2, float y2)
{
	m_X1 = x1;
	m_Y1 = y1;
	m_X2 = x2;
	m_Y2 = y2;
	for (int i = 0; i < SplineTableSize; ++i)
	{
		m_Values[i] = calcBezier(i * SampleStepSize, x1, x2);
	}
}

float CubicEase::getT(float x) const
{
	float intervalStart = 0.0f;
	int currentSample = 1;
	int lastSample = SplineTableSize - 1;

	for (; currentSample != lastSample && m_Values[currentSample] <= x; ++currentSample)
	{
		intervalStart += SampleStepSize;
	}
	--currentSample;

	// Interpolate to provide an initial guess for t
	float dist = (x - m_Values[currentSample]) / (m_Values[currentSample + 1] - m_Values[currentSample]);
	float guessForT = intervalStart + dist * SampleStepSize;

	float initialSlope = getSlope(guessForT, m_X1, m_X2);
	if (initialSlope >= NewtonMinSlope)
	{
		for (int i = 0; i < NewtonIterations; ++i)
		{
			float currentSlope = getSlope(guessForT, m_X1, m_X2);
			if (currentSlope == 0.0)
			{
				return guessForT;
			}
			float currentX = calcBezier(guessForT, m_X1, m_X2) - x;
			guessForT -= currentX / currentSlope;
		}
		return guessForT;
	}
	else if (initialSlope == 0.0)
	{
		return guessForT;
	}
	else
	{
		float aB = intervalStart + SampleStepSize;
		float currentX, currentT;
		int i = 0;
		do
		{
			currentT = intervalStart + (aB - intervalStart) / 2.0;
			currentX = calcBezier(currentT, m_X1, m_X2) - x;
			if (currentX > 0.0)
			{
				aB = currentT;
			}
			else
			{
				intervalStart = currentT;
			}
		} while (abs(currentX) > SubdivisionPrecision && ++i < SubdivisionMaxIterations);
		return currentT;
	}
}

float CubicEase::ease(float mix) const { return calcBezier(getT(mix), m_Y1, m_Y2); }