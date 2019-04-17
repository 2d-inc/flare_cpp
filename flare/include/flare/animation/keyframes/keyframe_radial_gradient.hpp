#ifndef _FLARE_KEYFRAMERADIALGRADIENT_HPP_
#define _FLARE_KEYFRAMERADIALGRADIENT_HPP_

#include "keyframe_with_interpolation.hpp"
#include <vector>

namespace flare
{
	class ActorComponent;

	class KeyFrameRadialGradient : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;

	private:
		std::vector<float> m_Value;

	public:
		const std::vector<float>& value() const { return m_Value; }
		bool read(BlockReader* reader, ActorComponent* component) override;
		void apply(ActorComponent* component, float mix) override;
		void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
} // namespace flare

#endif