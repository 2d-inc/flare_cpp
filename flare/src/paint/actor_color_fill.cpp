#include "flare/paint/actor_color_fill.hpp"
#include "flare/actor_shape.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor.hpp"

using namespace flare;

ActorColorFill::ActorColorFill() : ActorColor(ComponentType::ColorFill) {}

void ActorColorFill::copy(const ActorColorFill* fill, ActorArtboard* artboard)
{
	ActorColor::copy(fill, artboard);
	ActorFill::copy(fill, artboard);
}

ActorColorFill* ActorColorFill::read(ActorArtboard* artboard, BlockReader* reader, ActorColorFill* component)
{
	ActorColor::read(artboard, reader, component);
	ActorFill::read(artboard, reader, component);
	return component;
}

void ActorColorFill::onShapeChanged(ActorShape* from, ActorShape* to)
{
	if (from != nullptr)
	{
		from->removeFill(this);
	}
	if (to != nullptr)
	{
		to->addFill(this);
	}
}

ActorComponent* ActorColorFill::makeInstance(ActorArtboard* artboard) const
{
	ActorColorFill* instanceNode = artboard->actor()->makeColorFill();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorColorFill::initializeGraphics() {}