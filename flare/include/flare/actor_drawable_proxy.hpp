#ifndef _FLARE_ACTOR_DRAWABLE_PROXY_HPP_
#define _FLARE_ACTOR_DRAWABLE_PROXY_HPP_

namespace flare
{
	class ActorDrawable;
	class ActorDrawableProxy
	{
	private:
		ActorDrawable* m_Drawable;

	public:
		ActorDrawableProxy(ActorDrawable* drawable);
        virtual ~ActorDrawableProxy() {}
		ActorDrawable* drawable() const { return m_Drawable; }
	};
} // namespace flare
#endif