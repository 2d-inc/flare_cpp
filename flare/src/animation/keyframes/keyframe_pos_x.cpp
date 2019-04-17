#include "flare/animation/keyframes/keyframe_pos_x.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFramePosX::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->x(node->x() * (1.0f - mix) + value * mix);
}