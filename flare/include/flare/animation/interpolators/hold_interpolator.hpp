#ifndef _FLARE_HOLD_INTERPOLATOR_HPP_
#define _FLARE_HOLD_INTERPOLATOR_HPP_

#include "keyframe_interpolator.hpp"

namespace flare
{
	class HoldInterpolator : public KeyFrameInterpolator
	{
	public:
		float getEasedMix(float mix) const override {
            return 0.0;
        }
        static const HoldInterpolator* instance()
        {
            static HoldInterpolator interpolator;
            return &interpolator;
        }
	};
} // namespace flare

#endif
