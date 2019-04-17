#ifndef _FLARE_ACTORTARGETEDCONSTRAINT_HPP_
#define _FLARE_ACTORTARGETEDCONSTRAINT_HPP_

#include "actor_constraint.hpp"

namespace flare
{
	class ActorTargetedConstraint : public ActorConstraint
	{
		typedef ActorConstraint Base;

	protected:
		union {
			unsigned short m_TargetIdx;
			ActorComponent* m_Target;
		};
		ActorTargetedConstraint(ActorArtboard* artboard, ComponentType type);

	public:
		ActorComponent* target() const { return m_Target; }

		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		void copy(const ActorTargetedConstraint* constraint, ActorArtboard* artboard);
		static ActorTargetedConstraint* read(ActorArtboard* artboard, BlockReader* reader,
		                                     ActorTargetedConstraint* constraint = nullptr);
	};
} // namespace flare

#endif