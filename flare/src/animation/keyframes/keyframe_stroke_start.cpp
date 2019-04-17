#include "flare/animation/keyframes/keyframe_stroke_start.hpp"
#include "flare/actor_component.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

void KeyFrameStrokeStart::setValue(ActorComponent* component, float value, float mix)
{
	ActorStroke* stroke = dynamic_cast<ActorStroke*>(component);
	if (stroke == nullptr)
	{
		return;
	}
	stroke->trimStart(stroke->trimStart() * (1.0f - mix) + value * mix);
}