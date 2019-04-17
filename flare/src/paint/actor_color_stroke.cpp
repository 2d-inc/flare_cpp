#include "flare/paint/actor_color_stroke.hpp"
#include "flare/actor_shape.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor.hpp"

using namespace flare;

ActorColorStroke::ActorColorStroke() : ActorColor(ComponentType::ColorStroke) {}

void ActorColorStroke::copy(const ActorColorStroke* fill, ActorArtboard* artboard)
{
	ActorColor::copy(fill, artboard);
	ActorStroke::copy(fill, artboard);
}

ActorColorStroke* ActorColorStroke::read(ActorArtboard* artboard, BlockReader* reader, ActorColorStroke* component)
{
	ActorColor::read(artboard, reader, component);
	ActorStroke::read(artboard, reader, component);
	return component;
}

void ActorColorStroke::onShapeChanged(ActorShape* from, ActorShape* to)
{
	if (from != nullptr)
	{
		from->removeStroke(this);
	}
	if (to != nullptr)
	{
		to->addStroke(this);
	}
}

ActorComponent* ActorColorStroke::makeInstance(ActorArtboard* artboard) const
{
	ActorColorStroke* instanceNode = artboard->actor()->makeColorStroke();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorColorStroke::initializeGraphics() {}
void ActorColorStroke::markPathEffectsDirty() {}