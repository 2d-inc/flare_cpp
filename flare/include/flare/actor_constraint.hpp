#ifndef _FLARE_ACTORCONSTRAINT_HPP_
#define _FLARE_ACTORCONSTRAINT_HPP_

#include "actor_component.hpp"

namespace flare
{
	class ActorConstraint : public ActorComponent
	{
		typedef ActorComponent Base;

	protected:
		bool m_IsEnabled;
		float m_Strength;

	protected:
		ActorConstraint(ActorArtboard* artboard, ComponentType type);

	public:
		bool isEnabled() const;
		float strength() const;
		void strength(float value);

		void onDirty(DirtyFlags dirt) override;
		virtual void constrain(ActorNode* node) = 0;
		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		void copy(const ActorConstraint* constraint, ActorArtboard* artboard);
		static ActorConstraint* read(ActorArtboard* artboard, BlockReader* reader, ActorConstraint* constraint = nullptr);
	};
} // namespace flare

#endif