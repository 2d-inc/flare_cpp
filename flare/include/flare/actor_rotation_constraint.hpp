#ifndef _FLARE_ACTORROTATIONCONSTRAINT_HPP_
#define _FLARE_ACTORROTATIONCONSTRAINT_HPP_

#include "actor_targeted_constraint.hpp"
#include "transform_space.hpp"
#include "flare_math/transform_components.hpp"

namespace flare
{
	class ActorRotationConstraint : public ActorTargetedConstraint
	{
		typedef ActorTargetedConstraint Base;

	private:
		bool m_Copy;
		float m_Scale;
		bool m_EnableMin;
		bool m_EnableMax;
		float m_Min;
		float m_Max;
		bool m_Offset;
		TransformSpace m_SourceSpace;
		TransformSpace m_DestSpace;
		TransformSpace m_MinMaxSpace;
		TransformComponents m_ComponentsA;
		TransformComponents m_ComponentsB;

	public:
		ActorRotationConstraint();
		void copy(const ActorRotationConstraint* node, ActorArtboard* artboard);
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		static ActorRotationConstraint* read(ActorArtboard* artboard, BlockReader* reader,
		                                     ActorRotationConstraint* constraint = nullptr);
		void constrain(ActorNode* node) override;
	};
} //namespace flare

#endif