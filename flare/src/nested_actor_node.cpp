#include "flare/nested_actor_node.hpp"
#include "flare/actor.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

NestedActorNode::NestedActorNode() :
    ActorDrawable(ComponentType::NestedActorNode),
    m_Asset(nullptr),
    m_ArtboardInstance(nullptr),
    m_TransformArtboardInstance(nullptr)
{
}

NestedActorNode::~NestedActorNode() { delete m_ArtboardInstance; }

ActorComponent* NestedActorNode::makeInstance(ActorArtboard* resetActor) const
{
	NestedActorNode* instanceNode = new NestedActorNode();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void NestedActorNode::setArtboardInstance(ActorArtboard* instance)
{
	m_ArtboardInstance = instance;
	m_TransformArtboardInstance = instance;
}

void NestedActorNode::transformNested(bool transformIt)
{
	m_TransformArtboardInstance = transformIt ? m_ArtboardInstance : nullptr;
}

void NestedActorNode::copy(const NestedActorNode* node, ActorArtboard* resetActor)
{
	Base::copy(node, resetActor);

	m_DrawOrder = node->m_DrawOrder;
	m_Asset = node->m_Asset;
	// todo: fix this
	// ActorArtboard* actor = m_Asset->actor();
	// if (actor != nullptr)
	// {
	// 	NestedActorNode* self = this;
	// 	self->setActorInstance(m_Asset->actor()->makeInstance());
	// }
}

void NestedActorNode::updateWorldTransform()
{
	Base::updateWorldTransform();
	if (m_TransformArtboardInstance != nullptr)
	{
		m_TransformArtboardInstance->root()->overrideWorldTransform(worldTransform());
	}
}

NestedActorNode* NestedActorNode::read(ActorArtboard* actor, BlockReader* reader, NestedActorNode* node)
{
	if (node == nullptr)
	{
		node = new NestedActorNode();
	}

	Base::read(actor, reader, node);

	bool isVisible = reader->readUint8() != 0;
	if (isVisible)
	{
		node->m_DrawOrder = (int)reader->readUint16();
		int assetIndex = (int)reader->readUint16();
		node->m_Asset = actor->nestedActorAsset(assetIndex);
	}
	return node;
}

void NestedActorNode::advance(float elapsedSeconds)
{
	if (m_ArtboardInstance != nullptr)
	{
		m_ArtboardInstance->advance(elapsedSeconds);
	}
}

void NestedActorNode::initializeGraphics() {}