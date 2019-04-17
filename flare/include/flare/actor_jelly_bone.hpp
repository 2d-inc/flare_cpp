#ifndef _FLARE_ACTORJELLYBONE_HPP_
#define _FLARE_ACTORJELLYBONE_HPP_

#include "actor_bone_base.hpp"

namespace flare
{
	class Actor;
	class BlockReader;

	class ActorJellyBone : public ActorBoneBase
	{
		typedef ActorBoneBase Base;

	public:
		ActorJellyBone();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		static ActorJellyBone* read(ActorArtboard* artboard, BlockReader* reader, ActorJellyBone* node = nullptr);
	};
} // namespace flare
#endif