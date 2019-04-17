#ifndef _FLARE_ACTOR_RADIAL_GRADIENT_STROKE_HPP_
#define _FLARE_ACTOR_RADIAL_GRADIENT_STROKE_HPP_

#include "flare/paint/actor_stroke.hpp"
#include "flare/paint/actor_radial_gradient.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorRadialGradientStroke : public ActorRadialGradient, public ActorStroke
	{
		typedef ActorRadialGradient Base;

	public:
		ActorRadialGradientStroke();
		void copy(const ActorRadialGradientStroke* gradientColor, ActorArtboard* artboard);
		static ActorRadialGradientStroke* read(ActorArtboard* artboard, BlockReader* reader, ActorRadialGradientStroke* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void initializeGraphics() override;
        void markPathEffectsDirty() override;
		void markPaintDirty() override { ActorRadialGradient::markPaintDirty(); }
		void validatePaint() override { ActorRadialGradient::validatePaint(); }
	};
} // namespace flare

#endif