#ifndef _FLARE_KEYFRAMEWITHINTERPOLATION_HPP_
#define _FLARE_KEYFRAMEWITHINTERPOLATION_HPP_

#include "keyframe.hpp"
#include "../interpolators/keyframe_interpolator.hpp"

namespace flare
{
	class KeyFrameWithInterpolation : public KeyFrame
	{
		typedef KeyFrame Base;

	protected:
		const KeyFrameInterpolator* m_Interpolator;

	public:
		KeyFrameWithInterpolation();
		virtual ~KeyFrameWithInterpolation();
		const KeyFrameInterpolator* interpolator() const;
		InterpolationType interpolationType() const;

		bool read(BlockReader* reader, ActorComponent* component) override;
		void setNext(KeyFrame* frame) override;
	};
} // namespace flare

#endif