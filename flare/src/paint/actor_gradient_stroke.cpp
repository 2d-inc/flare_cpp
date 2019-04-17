#include "flare/paint/actor_gradient_stroke.hpp"
#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorGradientStroke::ActorGradientStroke() : Base(ComponentType::GradientStroke) {}

void ActorGradientStroke::copy(const ActorGradientStroke* Stroke, ActorArtboard* artboard)
{
	ActorGradientColor::copy(Stroke, artboard);
	ActorStroke::copy(Stroke, artboard);
}

ActorGradientStroke* ActorGradientStroke::read(ActorArtboard* artboard, BlockReader* reader,
                                               ActorGradientStroke* component)
{
	ActorGradientColor::read(artboard, reader, component);
	ActorStroke::read(artboard, reader, component);
	return component;
}

void ActorGradientStroke::onShapeChanged(ActorShape* from, ActorShape* to)
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

ActorComponent* ActorGradientStroke::makeInstance(ActorArtboard* artboard) const
{
	ActorGradientStroke* instanceNode = artboard->actor()->makeGradientStroke();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorGradientStroke::initializeGraphics() {}
void ActorGradientStroke::markPathEffectsDirty() {}