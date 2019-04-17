#ifndef _FLARE_ACTORBONE_HPP_
#define _FLARE_ACTORBONE_HPP_

#include "actor_bone_base.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class JellyComponent;

	class ActorBone : public ActorBoneBase
	{
		typedef ActorBoneBase Base;
		friend class JellyComponent;

	protected:
		ActorBone* m_FirstBone;
		JellyComponent* m_Jelly;

	public:
		ActorBone();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void completeResolve() override;
		static ActorBone* read(ActorArtboard* artboard, BlockReader* reader, ActorBone* node = nullptr);
	};
} // namespace flare
#endif