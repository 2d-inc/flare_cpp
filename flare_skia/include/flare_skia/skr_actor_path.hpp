#ifndef _SKR_ACTOR_PATH_HPP_
#define _SKR_ACTOR_PATH_HPP_

#include "flare/path/actor_path.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorPath : public ActorPath, public SkrActorBasePath
	{
		typedef ActorPath Base;

	public:
		SkrActorPath();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif