#ifndef _FLARE_KEYFRAMEINTPROPERTY_HPP_
#define _FLARE_KEYFRAMEINTPROPERTY_HPP_

#include "keyframe_numeric.hpp"
#include <string>

namespace flare
{
	class ActorComponent;

	class KeyFrameIntProperty : public KeyFrameInt
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};

	class KeyFrameFloatProperty : public KeyFrameNumeric
	{
		protected:
			void setValue(ActorComponent* component, float value, float mix) override;
	};

	class KeyFrameStringProperty : public KeyFrame
	{
		typedef KeyFrame Base;
		private:
			std::string m_Value;

		public:	
			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};

	class KeyFrameBooleanProperty : public KeyFrame
	{
		typedef KeyFrame Base;
		private:
			bool m_Value;

		public:	
			bool read(BlockReader* reader, ActorComponent* component) override;
			void setNext(KeyFrame* frame) override;
			void apply(ActorComponent* component, float mix) override;
			void applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix) override;
	};
}

#endif