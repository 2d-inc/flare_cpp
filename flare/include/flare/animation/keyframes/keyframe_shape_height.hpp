#ifndef _FLARE_KEYFRAME_SHAPE_HEIGHT_HPP_
#define _FLARE_KEYFRAME_SHAPE_HEIGHT_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameShapeHeight : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif