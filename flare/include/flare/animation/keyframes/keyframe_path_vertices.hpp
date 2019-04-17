#ifndef _FLARE_KEYFRAMEPATHVERTICES_HPP_
#define _FLARE_KEYFRAMEPATHVERTICES_HPP_

#include "keyframe_with_interpolation.hpp"

namespace flare
{
	class ActorComponent;

	class KeyFramePathVertices : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			float* m_Vertices;
			unsigned int m_VerticesLength;

		public:	
			KeyFramePathVertices();
			virtual ~KeyFramePathVertices();

			bool read(BlockReader* reader, ActorComponent* component) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif