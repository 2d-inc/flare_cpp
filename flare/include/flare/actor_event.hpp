#ifndef _FLARE_ACTOREVENT_HPP_
#define _FLARE_ACTOREVENT_HPP_

#include "actor_component.hpp"
#include <string>

namespace flare
{
	class ActorEvent : public ActorComponent
	{
		typedef ActorComponent Base;

	public:
		ActorEvent();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorComponent* node, ActorArtboard* artboard);

		static ActorEvent* read(ActorArtboard* artboard, BlockReader* reader, ActorEvent* component = NULL);
	};
} // namespace flare
#endif