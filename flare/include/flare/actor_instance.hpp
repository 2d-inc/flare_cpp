#ifndef _FLARE_ACTORINSTANCE_HPP_
#define _FLARE_ACTORINSTANCE_HPP_

#include "actor.hpp"

namespace flare
{
	class ActorInstance : public Actor
	{
	public:
		ActorInstance(const Actor& actor);
		~ActorInstance();
	};
} // namespace flare
#endif