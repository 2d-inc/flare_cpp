#ifndef _FLARE_ACTORROOTBONE_HPP_
#define _FLARE_ACTORROOTBONE_HPP_

#include "actor_node.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorRootBone : public ActorNode
	{
	public:
		ActorRootBone();
		ActorNode* makeInstance(ActorArtboard* artboard) const;
		void copy(const ActorRootBone* node, ActorArtboard* artboard);

		static ActorRootBone* read(ActorArtboard* artboard, BlockReader* reader, ActorRootBone* node = NULL);
	};
} // namespace flare
#endif