#ifndef _FLARE_ACTORTRIANGLE_HPP_
#define _FLARE_ACTORTRIANGLE_HPP_

#include "actor_procedural_path.hpp"
#include <vector>

namespace flare
{
	class ActorTriangle : public ActorProceduralPath
	{
		typedef ActorProceduralPath Base;

	public:
		ActorTriangle();

		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void makePoints() override;
	};
} // namespace flare
#endif