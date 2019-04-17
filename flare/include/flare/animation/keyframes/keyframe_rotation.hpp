#ifndef _FLARE_KEYFRAMEROTATION_HPP_
#define _FLARE_KEYFRAMEROTATION_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameRotation : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif