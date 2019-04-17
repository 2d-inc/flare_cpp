#ifndef _FLARE_KEYFRAMELENGTH_HPP_
#define _FLARE_KEYFRAMELENGTH_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class ActorNode;

	class KeyFrameLength : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif