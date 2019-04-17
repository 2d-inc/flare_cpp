#ifndef _FLARE_ACTORTRANSFORMCONSTRAINT_HPP_
#define _FLARE_ACTORTRANSFORMCONSTRAINT_HPP_

#include "actor_targeted_constraint.hpp"
#include "transform_space.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_math/transform_components.hpp"
#include <vector>

namespace flare
{
	class Actor;

	class ActorTransformConstraint : public ActorTargetedConstraint
	{
		typedef ActorTargetedConstraint Base;

	private:
		TransformSpace m_SourceSpace;
		TransformSpace m_DestSpace;
		TransformComponents m_ComponentsA;
		TransformComponents m_ComponentsB;

	public:
		ActorTransformConstraint();

		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorTransformConstraint* node, ActorArtboard* artboard);
		void constrain(ActorNode* node) override;

		static ActorTransformConstraint* read(ActorArtboard* artboard, BlockReader* reader,
		                                      ActorTransformConstraint* node = nullptr);
	};
} //namespace flare

#endif