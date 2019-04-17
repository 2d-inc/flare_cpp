#include "flare/paint/actor_gradient_fill.hpp"
#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorGradientFill::ActorGradientFill() : Base(ComponentType::GradientFill) {}

void ActorGradientFill::copy(const ActorGradientFill* fill, ActorArtboard* artboard)
{
	Base::copy(fill, artboard);
	ActorFill::copy(fill, artboard);
}

ActorGradientFill* ActorGradientFill::read(ActorArtboard* artboard, BlockReader* reader, ActorGradientFill* component)
{
	Base::read(artboard, reader, component);
	ActorFill::read(artboard, reader, component);
	return component;
}

void ActorGradientFill::onShapeChanged(ActorShape* from, ActorShape* to)
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

ActorComponent* ActorGradientFill::makeInstance(ActorArtboard* artboard) const
{
	ActorGradientFill* instanceNode = artboard->actor()->makeGradientFill();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorGradientFill::initializeGraphics() {}