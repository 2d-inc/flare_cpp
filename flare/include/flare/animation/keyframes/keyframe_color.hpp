#ifndef _FLARE_KEYFRAMECOLOR_HPP_
#define _FLARE_KEYFRAMECOLOR_HPP_

#include "keyframe_with_interpolation.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameColor : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;

	private:
		float m_Value[4];

	public:
		KeyFrameColor();
		const float* value() const { return m_Value; }
		bool read(BlockReader* reader, ActorComponent* component) override;
		void apply(ActorComponent* component, float mix) override;
		void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
} // namespace flare

#endif