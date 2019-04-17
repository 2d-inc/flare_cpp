#ifndef _FLARE_ACTOR_GRADIENT_STROKE_HPP_
#define _FLARE_ACTOR_GRADIENT_STROKE_HPP_

#include "flare/paint/actor_stroke.hpp"
#include "flare/paint/actor_gradient_color.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorGradientStroke : public ActorGradientColor, public ActorStroke
	{
		typedef ActorGradientColor Base;

	public:
		ActorGradientStroke();
		void copy(const ActorGradientStroke* gradientColor, ActorArtboard* artboard);
		static ActorGradientStroke* read(ActorArtboard* artboard, BlockReader* reader, ActorGradientStroke* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void initializeGraphics() override;
		void markPaintDirty() override { ActorGradientColor::markPaintDirty(); }
		void markPathEffectsDirty() override;
		void validatePaint() override { ActorGradientColor::validatePaint(); }
	};
} // namespace flare

#endif