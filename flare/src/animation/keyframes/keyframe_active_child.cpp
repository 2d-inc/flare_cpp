#include "flare/animation/keyframes/keyframe_active_child.hpp"
#include "flare/block_reader.hpp"
#include "flare/actor_node_solo.hpp"

using namespace flare;

bool KeyFrameActiveChild::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	m_Value = reader->readFloat32();
	return true;
}

void KeyFrameActiveChild::setNext(KeyFrame* frame)
{
	// Blank, no interpolation here
}

void KeyFrameActiveChild::apply(ActorComponent* component, float mix)
{
	// printf("APPLY KeyFrameActiveChild %d\n", m_Value);
	ActorNodeSolo* node = reinterpret_cast<ActorNodeSolo*>(component);
	node->activeChildIndex(m_Value);
}

void KeyFrameActiveChild::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	apply(component, mix);
}