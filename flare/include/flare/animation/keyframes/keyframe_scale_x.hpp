#ifndef _FLARE_KEYFRAMESCALEX_HPP_
#define _FLARE_KEYFRAMESCALEX_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameScaleX : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif