#ifndef _FLARE_KEYFRAME_PAINT_OPACITY_HPP_
#define _FLARE_KEYFRAME_PAINT_OPACITY_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFramePaintOpacity : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif