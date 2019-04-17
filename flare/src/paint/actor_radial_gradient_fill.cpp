#include "flare/paint/actor_radial_gradient_fill.hpp"
#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorRadialGradientFill::ActorRadialGradientFill() : Base(ComponentType::RadialGradientFill) {}

void ActorRadialGradientFill::copy(const ActorRadialGradientFill* fill, ActorArtboard* artboard)
{
	Base::copy(fill, artboard);
	ActorFill::copy(fill, artboard);
}

ActorRadialGradientFill* ActorRadialGradientFill::read(ActorArtboard* artboard, BlockReader* reader, ActorRadialGradientFill* component)
{
	Base::read(artboard, reader, component);
	ActorFill::read(artboard, reader, component);
	return component;
}

void ActorRadialGradientFill::onShapeChanged(ActorShape* from, ActorShape* to)
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

ActorComponent* ActorRadialGradientFill::makeInstance(ActorArtboard* artboard) const
{
	ActorRadialGradientFill* instanceNode = artboard->actor()->makeRadialGradientFill();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorRadialGradientFill::initializeGraphics() {}