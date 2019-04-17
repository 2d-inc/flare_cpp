#ifndef _FLARE_KEYFRAME_STROKE_OFFSET_HPP_
#define _FLARE_KEYFRAME_STROKE_OFFSET_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameStrokeOffset : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif