#ifndef _FLARE_ACTOR_GRADIENT_FILL_HPP_
#define _FLARE_ACTOR_GRADIENT_FILL_HPP_

#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_gradient_color.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorGradientFill : public ActorGradientColor, public ActorFill
	{
		typedef ActorGradientColor Base;

	public:
		ActorGradientFill();
		void copy(const ActorGradientFill* gradientColor, ActorArtboard* artboard);
		static ActorGradientFill* read(ActorArtboard* artboard, BlockReader* reader, ActorGradientFill* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void initializeGraphics() override;
		void validatePaint() override { ActorGradientColor::validatePaint(); }
		void markPaintDirty() override { ActorGradientColor::markPaintDirty(); }
	};
} // namespace flare

#endif