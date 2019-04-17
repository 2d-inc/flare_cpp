#include "flare/animation/keyframes/keyframe_trigger.hpp"
#include "flare/actor.hpp"
#include "flare/actor_image.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

			
KeyFrameTrigger::KeyFrameTrigger()
{

}

KeyFrameTrigger::~KeyFrameTrigger()
{
}


bool KeyFrameTrigger::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}
	
	return true;
}

void KeyFrameTrigger::setNext(KeyFrame* frame)
{
	// Do nothing, we don't interpolate.
}

void KeyFrameTrigger::apply(ActorComponent* component, float mix)
{
	// Intentionally blank.
}

void KeyFrameTrigger::applyInterpolation(ActorComponent* node, float time, KeyFrame* toFrame, float mix)
{
	// Do nothing, we don't interpolate.
}