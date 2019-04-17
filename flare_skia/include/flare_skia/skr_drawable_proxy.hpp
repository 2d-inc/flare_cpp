#ifndef _SKR_DRAWABLE_PROXY_HPP_
#define _SKR_DRAWABLE_PROXY_HPP_

#include "flare/actor_drawable_proxy.hpp"

namespace flare
{
    class SkrDrawable;

	class SkrDrawableProxy : public ActorDrawableProxy
	{
        typedef ActorDrawableProxy Base;

        SkrDrawable* m_SkrDrawable;
	public:
		SkrDrawableProxy(ActorDrawable* drawable);
        SkrDrawable* skrDrawable() const { return m_SkrDrawable; }
	};
} // namespace flare
#endif