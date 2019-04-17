#include "flare/actor_node_solo.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorNodeSolo::ActorNodeSolo() : ActorNode(ComponentType::ActorNodeSolo), m_ActiveChildIndex(0) {}

int ActorNodeSolo::activeChildIndex() const { return m_ActiveChildIndex; }

void ActorNodeSolo::activeChildIndex(int index)
{
	if (m_ActiveChildIndex != index)
	{
		setActiveChildIndex(index);
	}
}
void ActorNodeSolo::setActiveChildIndex(int index)
{
	int numChildren = m_Children.size();
	m_ActiveChildIndex = std::min(numChildren, std::max(0, index));
	for (int i = 0; i < numChildren; i++)
	{
		auto an = m_Children[i];
		bool cv = (i != (m_ActiveChildIndex - 1));
		an->collapsedVisibility(cv);
	}
}

ActorComponent* ActorNodeSolo::makeInstance(ActorArtboard* artboard) const
{
	ActorNodeSolo* soloInstance = new ActorNodeSolo();
	soloInstance->copy(this, artboard);

	return soloInstance;
}

void ActorNodeSolo::copy(const ActorNodeSolo* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);
	m_ActiveChildIndex = node->m_ActiveChildIndex;
}

ActorNodeSolo* ActorNodeSolo::read(ActorArtboard* artboard, BlockReader* reader, ActorNodeSolo* node)
{
	if (node == nullptr)
	{
		node = new ActorNodeSolo();
	}

	Base::read(artboard, reader, node);

	node->m_ActiveChildIndex = reader->readUint32();

	return node;
}

void ActorNodeSolo::completeResolve()
{
	Base::completeResolve();
	setActiveChildIndex(m_ActiveChildIndex);
}