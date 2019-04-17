#ifndef _FLARE_ACTOR_COLOR_HPP_
#define _FLARE_ACTOR_COLOR_HPP_

#include "flare/paint/actor_paint.hpp"
#include "flare_math/color.hpp"

namespace flare
{
    class BlockReader;
    class ActorArtboard;

	class ActorColor : public ActorPaint
	{
		typedef ActorPaint Base;

	protected:
		Color m_Color;
        
	public:
        ActorColor(ComponentType type);
        const Color& color() const { return m_Color; }
        void color(const Color& value);
        void copy(const ActorColor* color, ActorArtboard* artboard);
        static ActorColor* read(ActorArtboard* artboard, BlockReader* reader, ActorColor* component);
	};
} // namespace flare

#endif