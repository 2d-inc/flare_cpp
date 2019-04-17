#ifndef _FLARE_ACTOR_COLOR_STROKE_HPP_
#define _FLARE_ACTOR_COLOR_STROKE_HPP_

#include "flare/paint/actor_color.hpp"
#include "flare/paint/actor_stroke.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorColorStroke : public ActorColor, public ActorStroke
	{
	public:
		ActorColorStroke();
		void copy(const ActorColorStroke* stroke, ActorArtboard* artboard);
		static ActorColorStroke* read(ActorArtboard* artboard, BlockReader* reader, ActorColorStroke* component);
		void onShapeChanged(ActorShape* from, ActorShape* to) override;
		void initializeGraphics() override;
		void markPathEffectsDirty() override;
		void markPaintDirty() override { ActorColor::markPaintDirty(); }
		void validatePaint() override { ActorColor::validatePaint(); }
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
	};
} // namespace flare

#endif