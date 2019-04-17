#include "flare/animation/keyframes/keyframe_stroke_offset.hpp"
#include "flare/actor_component.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

void KeyFrameStrokeOffset::setValue(ActorComponent* component, float value, float mix)
{
	ActorStroke* stroke = dynamic_cast<ActorStroke*>(component);
	if (stroke == nullptr)
	{
		return;
	}
	stroke->trimOffset(stroke->trimOffset() * (1.0f - mix) + value * mix);
}