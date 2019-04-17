#ifndef _FLARE_KEYFRAMEIMAGEVERTICES_HPP_
#define _FLARE_KEYFRAMEIMAGEVERTICES_HPP_

#include "keyframe_with_interpolation.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFrameImageVertices : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			float* m_Vertices;
			unsigned int m_VerticesCount;

		public:	
			KeyFrameImageVertices();
			virtual ~KeyFrameImageVertices();

			bool read(BlockReader* reader, ActorComponent* component) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif