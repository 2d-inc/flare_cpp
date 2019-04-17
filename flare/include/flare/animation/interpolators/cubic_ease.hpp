#ifndef _FLARE_CUBIC_EASE_HPP_
#define _FLARE_CUBIC_EASE_HPP_

namespace flare
{
    // Implements https://github.com/gre/bezier-easing/blob/master/src/index.js
	class CubicEase
	{
	private:
        static constexpr int SplineTableSize = 11;
        static constexpr float SampleStepSize = 1.0f / (SplineTableSize-1.0f);
		float m_X1, m_Y1, m_X2, m_Y2;
        float m_Values[SplineTableSize];

	public:
        CubicEase();
        CubicEase(float x1, float y1, float x2, float y2);
        void initialize(float x1, float y1, float x2, float y2);
		float ease(float t) const;
    private:
        float getT(float x) const;
	};
} // namespace flare

#endif
