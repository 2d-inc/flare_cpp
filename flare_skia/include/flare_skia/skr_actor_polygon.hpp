#ifndef _SKR_ACTOR_POLYGON_HPP_
#define _SKR_ACTOR_POLYGON_HPP_

#include "flare/path/actor_polygon.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorPolygon : public ActorPolygon, public SkrActorBasePath
	{
		typedef ActorPolygon Base;

	public:
		SkrActorPolygon();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif