#include "flare/animation/keyframes/keyframe_corner_radius.hpp"
#include "flare/actor_component.hpp"
#include "flare/path/actor_rectangle.hpp"

using namespace flare;

void KeyFrameCornerRadius::setValue(ActorComponent* component, float value, float mix)
{
	ActorRectangle* rectangle = dynamic_cast<ActorRectangle*>(component);
	if (rectangle == nullptr)
	{
		return;
	}
	rectangle->radius(rectangle->radius() * (1.0f - mix) + value * mix);
}