#ifndef _FLARE_KEYFRAME_CORNER_RADIUS_HPP_
#define _FLARE_KEYFRAME_CORNER_RADIUS_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameCornerRadius : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif