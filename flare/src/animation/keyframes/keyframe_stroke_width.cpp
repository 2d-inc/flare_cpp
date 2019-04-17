#include "flare/animation/keyframes/keyframe_stroke_width.hpp"
#include "flare/actor_component.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

void KeyFrameStrokeWidth::setValue(ActorComponent* component, float value, float mix)
{
	ActorStroke* stroke = dynamic_cast<ActorStroke*>(component);
	if (stroke == nullptr)
	{
		return;
	}
	stroke->width(stroke->width() * (1.0f - mix) + value * mix);
}