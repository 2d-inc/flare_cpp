#include "flare/animation/keyframes/keyframe_scale_x.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFrameScaleX::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->scaleX(node->scaleX() * (1.0f - mix) + value * mix);
}