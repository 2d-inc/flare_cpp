#ifndef _SKR_ACTOR_TRIANGLE_HPP_
#define _SKR_ACTOR_TRIANGLE_HPP_

#include "flare/path/actor_triangle.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorTriangle : public ActorTriangle, public SkrActorBasePath
	{
		typedef ActorTriangle Base;

	public:
		SkrActorTriangle();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif