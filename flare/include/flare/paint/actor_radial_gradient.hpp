#ifndef _FLARE_ACTOR_RADIAL_GRADIENT_HPP_
#define _FLARE_ACTOR_RADIAL_GRADIENT_HPP_

#include "flare/paint/actor_gradient_color.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;
	class KeyFrameRadialGradient;

	class ActorRadialGradient : public ActorGradientColor
	{
		friend class KeyFrameRadialGradient;
		typedef ActorGradientColor Base;

	private:
		float m_SecondaryRadiusScale;

	public:
		ActorRadialGradient(ComponentType type);
		float secondaryRadiusScale() const;
		void secondaryRadiusScale(float value);
		void copy(const ActorRadialGradient* stroke, ActorArtboard* artboard);
		static ActorRadialGradient* read(ActorArtboard* artboard, BlockReader* reader, ActorRadialGradient* component);
	};
} // namespace flare

#endif