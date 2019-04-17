#include "flare/actor_node.hpp"
#include "flare/actor.hpp"
#include "flare/block_reader.hpp"
#include <algorithm>
#include <cassert>
using namespace flare;

ActorNode::ActorNode(ActorArtboard* artboard) : ActorNode(artboard, ComponentType::ActorNode) {}

ActorNode::ActorNode(ComponentType type) : ActorNode(nullptr, type) {}

ActorNode::ActorNode(ActorArtboard* artboard, ComponentType type) :
    ActorComponent(artboard, type),
    m_Rotation(0.0f),
    m_Scale(1.0f, 1.0f),
    m_Opacity(1.0f),
    m_RenderOpacity(1.0f),
    m_OverrideWorldTransform(false),
    m_IsCollapsedVisibility(false),
    m_RenderCollapsed(false)
{
}

ActorNode::ActorNode() : ActorNode(nullptr, ComponentType::ActorNode) {}

ActorNode::~ActorNode() {}

const Mat2D& ActorNode::transform() const { return m_Transform; }

const Mat2D& ActorNode::worldTransform() const { return m_WorldTransform; }

Mat2D& ActorNode::mutableWorldTransform() { return m_WorldTransform; }

Mat2D& ActorNode::mutableTransform() { return m_Transform; }

void ActorNode::overrideWorldTransform(const Mat2D& transform)
{
	m_OverrideWorldTransform = true;
	Mat2D::copy(m_WorldTransform, transform);
	markTransformDirty();
}

void ActorNode::clearWorldTransformOverride()
{
	if (!m_OverrideWorldTransform)
	{
		return;
	}
	m_OverrideWorldTransform = false;
	markTransformDirty();
}

const Vec2D& ActorNode::translation() const { return m_Translation; }

void ActorNode::translation(const Vec2D& v)
{
	if (m_Translation[0] == v[0] && m_Translation[1] == v[1])
	{
		return;
	}
	Vec2D::copy(m_Translation, v);
	markTransformDirty();
}

float ActorNode::x() const { return m_Translation[0]; }

void ActorNode::x(float v)
{
	if (m_Translation[0] == v)
	{
		return;
	}
	m_Translation[0] = v;
	markTransformDirty();
}

float ActorNode::y() const { return m_Translation[1]; }

void ActorNode::y(float v)
{
	if (m_Translation[1] == v)
	{
		return;
	}
	m_Translation[1] = v;
	markTransformDirty();
}

float ActorNode::scaleX() const { return m_Scale[0]; }

void ActorNode::scaleX(float v)
{
	if (m_Scale[0] == v)
	{
		return;
	}
	m_Scale[0] = v;
	markTransformDirty();
}

float ActorNode::scaleY() const { return m_Scale[1]; }

void ActorNode::scaleY(float v)
{
	if (m_Scale[1] == v)
	{
		return;
	}
	m_Scale[1] = v;
	markTransformDirty();
}

float ActorNode::rotation() const { return m_Rotation; }

void ActorNode::rotation(float v)
{
	if (m_Rotation == v)
	{
		return;
	}
	m_Rotation = v;
	markTransformDirty();
}

float ActorNode::opacity() const { return m_Opacity; }

void ActorNode::opacity(float v)
{
	if (m_Opacity == v)
	{
		return;
	}
	m_Opacity = v;
	markTransformDirty();
}

float ActorNode::renderOpacity() const { return m_RenderOpacity; }

bool ActorNode::collapsedVisibility() const { return m_IsCollapsedVisibility; }

void ActorNode::collapsedVisibility(bool v)
{
	if (m_IsCollapsedVisibility != v)
	{
		m_IsCollapsedVisibility = v;
		markTransformDirty();
	}
}

bool ActorNode::renderCollapsed() const { return m_RenderCollapsed; }

void ActorNode::markTransformDirty()
{
	if (m_Artboard == nullptr)
	{
		// Still loading?
		return;
	}
	if (!m_Artboard->addDirt(this, DirtyFlags::TransformDirty))
	{
		return;
	}
	m_Artboard->addDirt(this, DirtyFlags::WorldTransformDirty, true);
}

Vec2D ActorNode::worldTranslation()
{
	Vec2D result;
	result[0] = m_WorldTransform[4];
	result[1] = m_WorldTransform[5];
	return result;
}

void ActorNode::worldTranslation(Vec2D& result)
{
	result[0] = m_WorldTransform[4];
	result[1] = m_WorldTransform[5];
}

void ActorNode::updateTransform()
{
	Mat2D::fromRotation(m_Transform, m_Rotation);
	m_Transform[4] = m_Translation[0];
	m_Transform[5] = m_Translation[1];
	Mat2D::scale(m_Transform, m_Transform, m_Scale);
}

void ActorNode::updateWorldTransform()
{
	double opacity = m_Opacity;

	if (m_Parent != nullptr)
	{
		m_RenderCollapsed = (m_IsCollapsedVisibility || m_Parent->renderCollapsed());
		opacity *= m_Parent->renderOpacity();
		if (!m_OverrideWorldTransform)
		{
			Mat2D::multiply(m_WorldTransform, m_Parent->m_WorldTransform, m_Transform);
		}
	}
	else
	{
		Mat2D::copy(m_WorldTransform, m_Transform);
	}

	if (opacity != m_RenderOpacity)
	{
		m_RenderOpacity = opacity;
		onRenderOpacityChanged();
	}
}

void ActorNode::addChild(ActorNode* node)
{
	assert(node != nullptr);
	if (node->m_Parent != nullptr)
	{
		node->m_Parent->removeChild(node);
	}
	node->m_Parent = this;
	m_Children.push_back(node);
	if (m_Artboard != nullptr)
	{
		m_Artboard->addDirt(this, DirtyFlags::ChildrenDirty, false);
	}
}

void ActorNode::removeChild(ActorNode* node)
{
	auto itr = std::find(m_Children.begin(), m_Children.end(), node);
	if (itr == m_Children.end())
	{
		return;
	}
	m_Children.erase(itr);
	node->m_Parent = nullptr;
	if (m_Artboard != nullptr)
	{
		m_Artboard->addDirt(this, DirtyFlags::ChildrenDirty, false);
	}
}

void ActorNode::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);

	for (auto it = m_Clips.begin(); it != m_Clips.end(); ++it)
	{
		ActorComponent* component = components[it->index];
		if ((it->node = static_cast<ActorNode*>(component)) == nullptr)
		{
			// Didn't resolve, delete this clip.
			it = m_Clips.erase(it);
		}
	}

	onClipsChanged();
}

ActorComponent* ActorNode::makeInstance(ActorArtboard* resetActor) const
{
	ActorNode* instanceNode = new ActorNode();
	instanceNode->copy(this, resetActor);
	return instanceNode;
}

void ActorNode::copy(const ActorNode* node, ActorArtboard* resetActor)
{
	Base::copy(node, resetActor);
	Mat2D::copy(m_Transform, node->m_Transform);
	Mat2D::copy(m_WorldTransform, node->m_WorldTransform);
	Vec2D::copy(m_Translation, node->m_Translation);
	Vec2D::copy(m_Scale, node->m_Scale);
	m_Rotation = node->m_Rotation;
	m_Opacity = node->m_Opacity;
	m_RenderOpacity = node->m_RenderOpacity;
	m_OverrideWorldTransform = node->m_OverrideWorldTransform;
}

ActorNode* ActorNode::read(ActorArtboard* artboard, BlockReader* reader, ActorNode* node)
{
	Base::read(artboard, reader, node);

	reader->read(node->m_Translation);
	node->m_Rotation = reader->readFloat32();
	reader->read(node->m_Scale);
	node->m_Opacity = reader->readFloat32();
	node->m_IsCollapsedVisibility = reader->readUint8() == 1;

	int clipCount = reader->readUint8();
	for (int i = 0; i < clipCount; i++)
	{
		node->m_Clips.emplace_back(ActorNodeClip(reader->readUint16()));
	}
	return node;
}

bool ActorNode::addConstraint(ActorConstraint* constraint)
{
	auto itr = std::find(m_Constraints.begin(), m_Constraints.end(), constraint);
	if (itr != m_Constraints.end())
	{
		return false;
	}
	m_Constraints.push_back(constraint);
	return true;
}

bool ActorNode::addPeerConstraint(ActorConstraint* constraint)
{
	auto itr = std::find(m_PeerConstraints.begin(), m_PeerConstraints.end(), constraint);
	if (itr != m_PeerConstraints.end())
	{
		return false;
	}
	m_PeerConstraints.push_back(constraint);
	return true;
}

std::vector<ActorConstraint*> ActorNode::allConstraints() const
{
	std::vector<ActorConstraint*> all;
	all.reserve(m_Constraints.size() + m_PeerConstraints.size());
	all.insert(all.end(), m_Constraints.begin(), m_Constraints.end());
	all.insert(all.end(), m_PeerConstraints.begin(), m_PeerConstraints.end());
	return all;
}

void ActorNode::update(DirtyFlags dirt)
{
	if ((dirt & DirtyFlags::TransformDirty) == DirtyFlags::TransformDirty)
	{
		updateTransform();
	}
	if ((dirt & DirtyFlags::WorldTransformDirty) == DirtyFlags::WorldTransformDirty)
	{
		updateWorldTransform();
		for (auto constraint : m_Constraints)
		{
			if (constraint->isEnabled())
			{
				constraint->constrain(this);
			}
		}
	}
	if ((dirt & DirtyFlags::ChildrenDirty) == DirtyFlags::ChildrenDirty)
	{
		onChildrenChanged();
	}
}