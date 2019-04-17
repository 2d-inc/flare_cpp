#ifndef _FLARE_KEYFRAMEPOSY_HPP_
#define _FLARE_KEYFRAMEPOSY_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFramePosY : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif