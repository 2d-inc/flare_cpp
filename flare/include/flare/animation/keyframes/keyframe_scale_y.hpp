#ifndef _FLARE_KEYFRAMESCALEY_HPP_
#define _FLARE_KEYFRAMESCALEY_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameScaleY : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif