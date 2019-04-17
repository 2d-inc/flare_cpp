#ifndef _FLARE_ACTORAXISCONSTRAINT_HPP_
#define _FLARE_ACTORAXISCONSTRAINT_HPP_

#include "actor_bone.hpp"
#include "actor_targeted_constraint.hpp"
#include "transform_space.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_math/transform_components.hpp"
#include <vector>

namespace flare
{
	class Actor;

	class ActorAxisConstraint : public ActorTargetedConstraint
	{
		typedef ActorTargetedConstraint Base;

	protected:
		bool m_CopyX;
		bool m_CopyY;
		float m_ScaleX;
		float m_ScaleY;
		bool m_EnableMinX;
		bool m_EnableMinY;
		bool m_EnableMaxX;
		bool m_EnableMaxY;
		float m_MaxX;
		float m_MaxY;
		float m_MinX;
		float m_MinY;
		bool m_Offset;
		TransformSpace m_SourceSpace;
		TransformSpace m_DestSpace;
		TransformSpace m_MinMaxSpace;

	public:
		ActorAxisConstraint(ActorArtboard* artboard, ComponentType type);
		void copy(const ActorAxisConstraint* node, ActorArtboard* artboard);

		static ActorAxisConstraint* read(ActorArtboard* artboard, BlockReader* reader, ActorAxisConstraint* node);
	};
} // namespace flare

#endif