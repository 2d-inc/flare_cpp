#ifndef _FLARE_KEYFRAMEACTIVECHILD_HPP_
#define _FLARE_KEYFRAMEACTIVECHILD_HPP_

#include "keyframe.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameActiveChild : public KeyFrame
	{
		typedef KeyFrame Base;
		
		private:
			unsigned m_Value;

		protected:
			void setValue(ActorComponent* component, float value, float mix);

		public:
			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;

	};
}

#endif