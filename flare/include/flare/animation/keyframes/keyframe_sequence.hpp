#ifndef _FLARE_KEYFRAMESEQUENCE_HPP_
#define _FLARE_KEYFRAMESEQUENCE_HPP_

#include "keyframe_numeric.hpp"

namespace flare
{
	class KeyFrameSequence : public KeyFrameNumeric
	{
		public:
            bool read(BlockReader* reader, ActorComponent* component) override;
            void setValue(ActorComponent* component, float value, float mix) override;
	};
}

#endif