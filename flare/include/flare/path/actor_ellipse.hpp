#ifndef _FLARE_ACTORELLIPSE_HPP_
#define _FLARE_ACTORELLIPSE_HPP_

#include "actor_procedural_path.hpp"
#include <vector>

namespace flare
{
	class ActorEllipse : public ActorProceduralPath
	{
		typedef ActorProceduralPath Base;

	public:
		ActorEllipse();

		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void makePoints() override;
	};
} // namespace flare
#endif