#ifndef _FLARE_ACTORANIMATION_HPP_
#define _FLARE_ACTORANIMATION_HPP_

#include <string>
#include <vector>
#include <functional>
#include "component_animation.hpp"

namespace flare
{
	class BlockReader;
	class ActorNode;
	class ActorEvent;

	struct ActorAnimationEvent
	{
		typedef std::function<void(const ActorAnimationEvent&, void*)> Callback;
		ActorEvent* actorEvent;
		float keyFrameTime;
		float elapsedTime;

		ActorAnimationEvent(ActorEvent* av, float time, float elapsed) :
			actorEvent(av),
			keyFrameTime(time),
			elapsedTime(elapsed)
		{

		}
	};

	class ActorAnimation
	{
		private:
			std::string m_Name;
			int m_FPS;
			float m_Duration;
			bool m_IsLooping;
			std::vector<ComponentAnimation*> m_AnimatedComponents;
			std::vector<ComponentAnimation*> m_TriggerComponents;

		public:

			ActorAnimation();
			~ActorAnimation();
			const std::string& name() const;
			float duration() const;
			bool isLooping() const;
			int fps() const;

			void read(BlockReader* reader, ActorComponent** components, int componentCount);

			void apply(float time, ActorArtboard* artboard, float mix);

			void triggerEvents(ActorArtboard* artboard, float fromTime, float toTime, std::vector<ActorAnimationEvent>& events);

			void fps(const int fps);
			void duration(const float duration);
	};
}
#endif