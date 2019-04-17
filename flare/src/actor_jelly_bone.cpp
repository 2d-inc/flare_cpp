#include "flare/actor_jelly_bone.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorJellyBone::ActorJellyBone() : Base(ComponentType::ActorJellyBone) {}

ActorComponent* ActorJellyBone::makeInstance(ActorArtboard* artboard) const
{
	ActorJellyBone* instanceNode = new ActorJellyBone();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

ActorJellyBone* ActorJellyBone::read(ActorArtboard* artboard, BlockReader* reader, ActorJellyBone* node)
{
	if (node == nullptr)
	{
		node = new ActorJellyBone();
	}

	// The Jelly Bone has a specialized read that doesn't go down the typical node path, this is
	// because majority of the transform properties of the Jelly Bone are controlled by the Jelly
	// Controller and are unnecessary for serialization.
	ActorComponent::read(artboard, reader, node);
	node->m_Opacity = reader->readFloat32();
	node->m_IsCollapsedVisibility = reader->readUint8() == 1;
	return node;
}