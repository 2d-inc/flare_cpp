#ifndef _SKR_ACTOR_ELLIPSE_HPP_
#define _SKR_ACTOR_ELLIPSE_HPP_

#include "flare/path/actor_ellipse.hpp"
#include "skr_actor_base_path.hpp"

namespace flare
{

	class SkrActorEllipse : public ActorEllipse, public SkrActorBasePath
	{
		typedef ActorEllipse Base;

	public:
		SkrActorEllipse();

		void invalidateDrawable() override;
	};
} // namespace flare

#endif