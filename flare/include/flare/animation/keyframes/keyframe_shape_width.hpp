#ifndef _FLARE_KEYFRAME_SHAPE_WIDTH_HPP_
#define _FLARE_KEYFRAME_SHAPE_WIDTH_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameShapeWidth : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif