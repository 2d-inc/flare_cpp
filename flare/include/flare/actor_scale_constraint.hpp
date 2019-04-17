#ifndef _FLARE_ACTORSCALECONSTRAINT_HPP_
#define _FLARE_ACTORSCALECONSTRAINT_HPP_

#include "actor_axis_constraint.hpp"

namespace flare
{
	class ActorScaleConstraint : public ActorAxisConstraint
	{
		typedef ActorAxisConstraint Base;

	private:
		TransformComponents m_ComponentsA;
		TransformComponents m_ComponentsB;

	public:
		ActorScaleConstraint();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		static ActorScaleConstraint* read(ActorArtboard* artboard, BlockReader* reader,
		                                  ActorScaleConstraint* constraint = nullptr);
		void constrain(ActorNode* node) override;
	};
} //namespace flare

#endif