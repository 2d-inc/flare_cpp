#include "flare/animation/keyframes/keyframe_length.hpp"
#include "flare/actor_bone.hpp"

using namespace flare;

void KeyFrameLength::setValue(ActorComponent* component, float value, float mix)
{
	// Hard assumption this is a bone.
	ActorBone* bone = reinterpret_cast<ActorBone*>(component);
	bone->length(bone->length() * (1.0f - mix) + value * mix);
}