#ifndef _FLARE_ACTOR_COLOR_FILL_HPP_
#define _FLARE_ACTOR_COLOR_FILL_HPP_

#include "flare/paint/actor_color.hpp"
#include "flare/paint/actor_fill.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorColorFill : public ActorColor, public ActorFill
	{
	public:
		ActorColorFill();
		void copy(const ActorColorFill* fill, ActorArtboard* artboard);
		static ActorColorFill* read(ActorArtboard* artboard, BlockReader* reader, ActorColorFill* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
		void initializeGraphics() override;
		void markPaintDirty() override { ActorColor::markPaintDirty(); }
		void validatePaint() override { ActorColor::validatePaint(); }
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;
	};
} // namespace flare

#endif