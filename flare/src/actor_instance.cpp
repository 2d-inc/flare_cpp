#include "flare/actor_instance.hpp"

using namespace flare;

ActorInstance::ActorInstance(const Actor& actor)
{
	copy(actor);
	/*
	 */
}

ActorInstance::~ActorInstance() {}