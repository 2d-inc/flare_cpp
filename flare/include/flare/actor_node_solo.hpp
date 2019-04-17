#ifndef _FLARE_ACTORNODESOLO_HPP_
#define _FLARE_ACTORNODESOLO_HPP_

#include "actor_node.hpp"
#include <string>

namespace flare
{
	class ActorNodeSolo : public ActorNode
	{
		typedef ActorNode Base;

	private:
		int m_ActiveChildIndex;

	public:
		ActorNodeSolo();
		int activeChildIndex() const;
		void activeChildIndex(int index);
		void setActiveChildIndex(int index);

		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorNodeSolo* node, ActorArtboard* artboard);
		void completeResolve() override;

		static ActorNodeSolo* read(ActorArtboard* artboard, BlockReader* reader, ActorNodeSolo* node = NULL);
	};
} // namespace flare

#endif