#ifndef _SKR_ACTOR_ARTBOARD_HPP_
#define _SKR_ACTOR_ARTBOARD_HPP_

#include "flare/actor_artboard.hpp"

class SkCanvas;

namespace flare
{
	class SkrActor;
	class SkrActorArtboard : public ActorArtboard
	{
        typedef Actor Base;
	public:
		SkrActorArtboard(SkrActor* actor);
		void initializeGraphics();
		void draw(SkCanvas* canvas);
	};
} // namespace flare
#endif