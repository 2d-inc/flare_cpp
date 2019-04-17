#include "flare/animation/keyframes/keyframe_paint_opacity.hpp"
#include "flare/actor_component.hpp"
#include "flare/paint/actor_paint.hpp"

using namespace flare;

void KeyFramePaintOpacity::setValue(ActorComponent* component, float value, float mix)
{
	ActorPaint* stroke = dynamic_cast<ActorPaint*>(component);
	if (stroke == nullptr)
	{
		return;
	}
	stroke->opacity(stroke->opacity() * (1.0f - mix) + value * mix);
}