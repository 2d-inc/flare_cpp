#include "flare/actor_root_bone.hpp"

using namespace flare;

ActorRootBone::ActorRootBone() : ActorNode(ComponentType::ActorRootBone) {}

ActorNode* ActorRootBone::makeInstance(ActorArtboard* artboard) const
{
	ActorRootBone* instanceNode = new ActorRootBone();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorRootBone::copy(const ActorRootBone* node, ActorArtboard* artboard) { ActorNode::copy(node, artboard); }

ActorRootBone* ActorRootBone::read(ActorArtboard* artboard, BlockReader* reader, ActorRootBone* node)
{
	if (node == nullptr)
	{
		node = new ActorRootBone();
	}

	ActorNode::read(artboard, reader, node);

	return node;
}