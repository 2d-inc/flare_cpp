#include "flare/animation/keyframes/keyframe_is_collision_enabled.hpp"
#include "flare/block_reader.hpp"
#include "flare/actor_collider.hpp"

using namespace flare;

bool KeyFrameIsCollisionEnabled::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	m_Value = reader->readUint8() == 1;
	return true;
}

void KeyFrameIsCollisionEnabled::setNext(KeyFrame* frame)
{
	// Intentionally blank, we do not interpolate.
}

void KeyFrameIsCollisionEnabled::apply(ActorComponent* component, float mix)
{
	ActorCollider* collider = reinterpret_cast<ActorCollider*>(component);
	collider->isCollisionEnabled(m_Value);
}

void KeyFrameIsCollisionEnabled::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	apply(component, mix);
}