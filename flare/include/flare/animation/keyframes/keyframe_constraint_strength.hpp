#ifndef _FLARE_KEYFRAMECONSTRAINTSTRENGTH_HPP_
#define _FLARE_KEYFRAMECONSTRAINTSTRENGTH_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameConstraintStrength : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif