#ifndef _FLARE_KEYFRAMEOPACITY_HPP_
#define _FLARE_KEYFRAMEOPACITY_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameOpacity : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif