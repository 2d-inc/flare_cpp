#ifndef _SKR_ACTOR_STAR_HPP_
#define _SKR_ACTOR_STAR_HPP_

#include "flare/path/actor_star.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorStar : public ActorStar, public SkrActorBasePath
	{
		typedef ActorStar Base;

	public:
		SkrActorStar();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif