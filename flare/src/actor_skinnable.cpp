#include "flare/actor_skinnable.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorSkinnable::SkinnedBone::SkinnedBone() : boneIndex(0) {}

ActorSkinnable::ActorSkinnable() : m_Skin(nullptr) {}

ActorSkin* ActorSkinnable::skin() const { return m_Skin; }

ActorSkinnable* ActorSkinnable::read(ActorArtboard* artboard, BlockReader* reader, ActorSkinnable* skinnable)
{
	int numConnectedBones = reader->readUint8();
	if (numConnectedBones != 0)
	{
		skinnable->m_ConnectedBones.reserve(numConnectedBones);
		for (int i = 0; i < numConnectedBones; i++)
		{
			SkinnedBone bc;
			bc.boneIndex = reader->readUint16();
			reader->read(bc.bind);
			Mat2D::invert(bc.inverseBind, bc.bind);
			skinnable->m_ConnectedBones.emplace_back(bc);
		}

		Mat2D worldOverride;
		reader->read(worldOverride);
		skinnable->overrideWorldTransform(worldOverride);
	}
	return skinnable;
}

void ActorSkinnable::resolveSkinnable(ActorComponent** components, int numComponents)
{
	for (auto& skinnedBone : m_ConnectedBones)
	{
		if (skinnedBone.boneIndex >= 0 && skinnedBone.boneIndex < numComponents)
		{
			skinnedBone.node = dynamic_cast<ActorNode*>(components[skinnedBone.boneIndex]);
		}
	}
}

void ActorSkinnable::copySkinnable(const ActorSkinnable* skinnable, ActorArtboard* artboard)
{
	m_ConnectedBones.clear();
	for (auto& from : skinnable->m_ConnectedBones)
	{
		SkinnedBone bc;
		bc.boneIndex = from.node == nullptr ? -1 : from.node->componentIndex();
		Mat2D::copy(bc.bind, from.bind);
		Mat2D::copy(bc.inverseBind, from.inverseBind);
		m_ConnectedBones.emplace_back(std::move(bc));
	}
}