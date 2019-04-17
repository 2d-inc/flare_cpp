#ifndef _FLARE_LINEAR_INTERPOLATOR_HPP_
#define _FLARE_LINEAR_INTERPOLATOR_HPP_

#include "keyframe_interpolator.hpp"

namespace flare
{
	class LinearInterpolator : public KeyFrameInterpolator
	{
	public:
		float getEasedMix(float mix) const override {
            return mix;
        }
        static const LinearInterpolator* instance()
        {
            static LinearInterpolator interpolator;
            return &interpolator;
        }
	};
} // namespace flare

#endif
