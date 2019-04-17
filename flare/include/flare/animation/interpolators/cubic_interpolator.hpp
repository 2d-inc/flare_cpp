#ifndef _FLARE_CUBIC_INTERPOLATOR_HPP_
#define _FLARE_CUBIC_INTERPOLATOR_HPP_

#include "keyframe_interpolator.hpp"
#include "flare/block_reader.hpp"
#include "cubic_ease.hpp"

namespace flare
{
	class CubicInterpolator : public KeyFrameInterpolator, public CubicEase
	{
	public:
		float getEasedMix(float mix) const override { return ease(mix); }
        bool read(BlockReader* reader);
	};
} // namespace flare

#endif
