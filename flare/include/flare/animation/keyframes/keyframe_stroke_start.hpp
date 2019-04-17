#ifndef _FLARE_KEYFRAME_STROKE_START_HPP_
#define _FLARE_KEYFRAME_STROKE_START_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameStrokeStart : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif