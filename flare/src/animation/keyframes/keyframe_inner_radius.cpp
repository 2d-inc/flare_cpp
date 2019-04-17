#include "flare/animation/keyframes/keyframe_inner_radius.hpp"
#include "flare/actor_component.hpp"
#include "flare/path/actor_star.hpp"

using namespace flare;

void KeyFrameInnerRadius::setValue(ActorComponent* component, float value, float mix)
{
	ActorStar* star = dynamic_cast<ActorStar*>(component);
	if (star == nullptr)
	{
		return;
	}
	star->innerRadius(star->innerRadius() * (1.0f - mix) + value * mix);
}