#ifndef _FLARE_COMPONENTANIMATION_HPP_
#define _FLARE_COMPONENTANIMATION_HPP_

#include <string>
#include <vector>
#include "property_animation.hpp"

namespace flare
{
	class BlockReader;
	class ActorComponent;
	class ActorArtboard;
	struct ActorAnimationEvent;

	class ComponentAnimation
	{
		private:
			unsigned short m_ComponentIndex;
			std::vector<PropertyAnimation*> m_Properties;

		public:
			ComponentAnimation();
			~ComponentAnimation();
			int componentIndex() const;

			void read(BlockReader* reader, ActorComponent** components);
			void apply(float time, ActorArtboard* artboard, float mix);
			void triggerEvents(ActorArtboard* artboard, float fromTime, float toTime, std::vector<ActorAnimationEvent>& events);

	};
}
#endif