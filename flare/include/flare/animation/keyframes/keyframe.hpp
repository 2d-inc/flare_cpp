#ifndef _FLARE_KEYFRAME_HPP_
#define _FLARE_KEYFRAME_HPP_

namespace flare
{
	class BlockReader;
	class ActorComponent;

	enum class InterpolationType
	{
		Hold = 0,
		Linear = 1,
		Cubic = 2,
		Max
	};

	class KeyFrame
	{
		public:

		protected:
			float m_Time;

		public:
			KeyFrame();
			virtual ~KeyFrame();
			
			float time() const;

			virtual bool read(BlockReader* reader, ActorComponent* component);
			virtual void setNext(KeyFrame* frame) = 0;
			virtual void apply(ActorComponent* component, float mix) = 0;
			virtual void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) = 0;
	};
}

#endif