#include "flare/paint/actor_radial_gradient_stroke.hpp"
#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorRadialGradientStroke::ActorRadialGradientStroke() : Base(ComponentType::RadialGradientStroke) {}

void ActorRadialGradientStroke::copy(const ActorRadialGradientStroke* stroke, ActorArtboard* artboard)
{
	Base::copy(stroke, artboard);
	ActorStroke::copy(stroke, artboard);
}

ActorRadialGradientStroke* ActorRadialGradientStroke::read(ActorArtboard* artboard, BlockReader* reader, ActorRadialGradientStroke* component)
{
	Base::read(artboard, reader, component);
	ActorStroke::read(artboard, reader, component);
	return component;
}

void ActorRadialGradientStroke::onShapeChanged(ActorShape* from, ActorShape* to)
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

ActorComponent* ActorRadialGradientStroke::makeInstance(ActorArtboard* artboard) const
{
	ActorRadialGradientStroke* instanceNode = artboard->actor()->makeRadialGradientStroke();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorRadialGradientStroke::initializeGraphics() {}
void ActorRadialGradientStroke::markPathEffectsDirty() {}