#include "flare/actor_bone.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorBone::ActorBone() : Base(ComponentType::ActorBone), m_FirstBone(nullptr), m_Jelly(nullptr) {}

ActorBone* ActorBone::read(ActorArtboard* artboard, BlockReader* reader, ActorBone* node)
{
	if (node == nullptr)
	{
		node = new ActorBone();
	}

	Base::read(artboard, reader, node);

	return node;
}

ActorComponent* ActorBone::makeInstance(ActorArtboard* artboard) const
{
	ActorBone* instanceNode = new ActorBone();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorBone::completeResolve()
{
	Base::completeResolve();
	for (ActorNode* node : m_Children)
	{
		if (node->type() == ComponentType::ActorBone)
		{
			ActorBone* bone = static_cast<ActorBone*>(node);
			if (bone != nullptr)
			{
				m_FirstBone = bone;
				break;
			}
		}
	}
}