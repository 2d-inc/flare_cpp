#include "flare/animation/keyframes/keyframe_shape_height.hpp"
#include "flare/actor_component.hpp"
#include "flare/path/actor_procedural_path.hpp"

using namespace flare;

void KeyFrameShapeHeight::setValue(ActorComponent* component, float value, float mix)
{
	ActorProceduralPath* path = dynamic_cast<ActorProceduralPath*>(component);
	if (path == nullptr)
	{
		return;
	}
	path->height(path->height() * (1.0f - mix) + value * mix);
}