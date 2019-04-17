#ifndef _FLARE_ACTORTRANSLATIONCONSTRAINT_HPP_
#define _FLARE_ACTORTRANSLATIONCONSTRAINT_HPP_

#include "actor_axis_constraint.hpp"

namespace flare
{
	class ActorTranslationConstraint : public ActorAxisConstraint
	{
		typedef ActorAxisConstraint Base;

	public:
		ActorTranslationConstraint();
		ActorComponent* makeInstance(ActorArtboard* resetActor) const override;
		static ActorTranslationConstraint* read(ActorArtboard* actor, BlockReader* reader,
		                                        ActorTranslationConstraint* constraint = nullptr);
		void constrain(ActorNode* node) override;
	};
} //namespace flare

#endif