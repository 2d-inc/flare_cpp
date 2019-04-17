#ifndef _FLARE_ACTORDISTANCECONSTRAINT_HPP_
#define _FLARE_ACTORDISTANCECONSTRAINT_HPP_

#include "actor_targeted_constraint.hpp"

namespace flare
{
	class ActorDistanceConstraint : public ActorTargetedConstraint
	{
		typedef ActorTargetedConstraint Base;

	private:
		enum class Mode
		{
			Closer = 0,
			Further = 1,
			Exact = 2
		};
		Mode m_Mode;
		float m_Distance;

	public:
		ActorDistanceConstraint();
		void copy(const ActorDistanceConstraint* node, ActorArtboard* artboard);
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		static ActorDistanceConstraint* read(ActorArtboard* artboard, BlockReader* reader,
		                                     ActorDistanceConstraint* constraint = nullptr);
		void constrain(ActorNode* node) override;
	};
} // namespace flare

#endif