#ifndef _FLARE_KEYFRAMENUMERIC_HPP_
#define _FLARE_KEYFRAMENUMERIC_HPP_

#include "keyframe_with_interpolation.hpp"

namespace flare
{
	class BlockReader;
	class ActorNode;

	class KeyFrameNumeric : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			float m_Value;

		public:
			KeyFrameNumeric();
			float value() const;

			bool read(BlockReader* reader, ActorComponent* component) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
			
		protected:
			virtual void setValue(ActorComponent* component, float value, float mix) = 0;
	};

	class KeyFrameInt : public KeyFrameWithInterpolation
	{
		typedef KeyFrameWithInterpolation Base;
		private:
			int m_Value;

		public:
			KeyFrameInt();
			int value() const;

			bool read(BlockReader* reader, ActorComponent* component) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
			
		protected:
			virtual void setValue(ActorComponent* component, float value, float mix) = 0;
	};
}

#endif