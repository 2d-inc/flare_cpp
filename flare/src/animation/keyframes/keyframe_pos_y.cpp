#include "flare/animation/keyframes/keyframe_pos_y.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFramePosY::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->y(node->y() * (1.0f - mix) + value * mix);
}