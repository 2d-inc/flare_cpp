#ifndef _FLARE_ACTOR_RADIAL_GRADIENT_FILL_HPP_
#define _FLARE_ACTOR_RADIAL_GRADIENT_FILL_HPP_

#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_radial_gradient.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorRadialGradientFill : public ActorRadialGradient, public ActorFill
	{
		typedef ActorRadialGradient Base;

	public:
		ActorRadialGradientFill();
		void copy(const ActorRadialGradientFill* gradientColor, ActorArtboard* artboard);
		static ActorRadialGradientFill* read(ActorArtboard* artboard, BlockReader* reader, ActorRadialGradientFill* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void initializeGraphics() override;
		void validatePaint() override { ActorGradientColor::validatePaint(); }
		void markPaintDirty() override { ActorRadialGradient::markPaintDirty(); }
	};
} // namespace flare

#endif