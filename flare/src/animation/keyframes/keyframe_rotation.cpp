#include "flare/animation/keyframes/keyframe_rotation.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFrameRotation::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->rotation(node->rotation() * (1.0f - mix) + value * mix);
}