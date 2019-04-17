#include "flare/animation/keyframes/keyframe_stroke_end.hpp"
#include "flare/actor_component.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

void KeyFrameStrokeEnd::setValue(ActorComponent* component, float value, float mix)
{
	ActorStroke* stroke = dynamic_cast<ActorStroke*>(component);
	if (stroke == nullptr)
	{
		return;
	}
	stroke->trimEnd(stroke->trimEnd() * (1.0f - mix) + value * mix);
}