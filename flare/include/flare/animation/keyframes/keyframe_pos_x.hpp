#ifndef _FLARE_KEYFRAMEPOSX_HPP_
#define _FLARE_KEYFRAMEPOSX_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFramePosX : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif