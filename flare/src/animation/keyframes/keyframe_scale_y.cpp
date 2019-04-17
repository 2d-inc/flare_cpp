#include "flare/animation/keyframes/keyframe_scale_y.hpp"
#include "flare/actor_node.hpp"

using namespace flare;

void KeyFrameScaleY::setValue(ActorComponent* component, float value, float mix)
{
	ActorNode* node = reinterpret_cast<ActorNode*>(component);
	node->scaleY(node->scaleY() * (1.0f - mix) + value * mix);
}