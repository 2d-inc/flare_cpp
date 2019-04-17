#ifndef _FLARE_ACTORANIMATIONINSTANCE_HPP_
#define _FLARE_ACTORANIMATIONINSTANCE_HPP_

#include <string>
#include <vector>
#include "actor_animation.hpp"

namespace flare
{
	class ActorAnimationInstance
	{
		private:
			ActorArtboard* m_Artboard;
			ActorAnimation* m_Animation;
			float m_Time;
			float m_Min;
			float m_Max;
			float m_Range;
			bool m_Loop;
			std::vector<ActorAnimationEvent> m_Events;
			void* m_EventCallbackUserData;
			ActorAnimationEvent::Callback m_EventCallback;

		public:

			ActorAnimationInstance(ActorArtboard* artboard, ActorAnimation* animation);
			~ActorAnimationInstance();

			float duration() const;
			float min() const;
			float max() const;
			float time() const;
			void time(float value);
			bool isLooping() const;
			void isLooping(bool isIt);

			void advance(float seconds);
			void apply(float mix);

			void eventCallback(ActorAnimationEvent::Callback callback, void* userdata = nullptr);

	};
}
#endif