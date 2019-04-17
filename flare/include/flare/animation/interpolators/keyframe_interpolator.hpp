#ifndef _FLARE_KEYFRAME_INTERPOLATOR_HPP_
#define _FLARE_KEYFRAME_INTERPOLATOR_HPP_

namespace flare
{
	class KeyFrameInterpolator
	{
	public:
		virtual ~KeyFrameInterpolator() {}
		virtual float getEasedMix(float mix) const = 0;
	};
} // namespace flare

#endif
