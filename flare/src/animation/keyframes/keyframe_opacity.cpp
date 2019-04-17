#include "flare/animation/keyframes/keyframe_opacity.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFrameOpacity::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->opacity(node->opacity() * (1.0f - mix) + value * mix);
}