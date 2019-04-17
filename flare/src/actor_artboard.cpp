#include "flare/actor_artboard.hpp"
#include "flare/actor.hpp"
#include "flare/actor_bone.hpp"
#include "flare/actor_collider.hpp"
#include "flare/actor_distance_constraint.hpp"
#include "flare/actor_drawable_proxy.hpp"
#include "flare/actor_event.hpp"
#include "flare/actor_flags.hpp"
#include "flare/actor_ik_constraint.hpp"
#include "flare/actor_jelly_bone.hpp"
#include "flare/actor_node_solo.hpp"
#include "flare/actor_root_bone.hpp"
#include "flare/actor_rotation_constraint.hpp"
#include "flare/actor_scale_constraint.hpp"
#include "flare/actor_shape.hpp"
#include "flare/actor_skin.hpp"
#include "flare/actor_transform_constraint.hpp"
#include "flare/actor_translation_constraint.hpp"
#include "flare/binary_reader.hpp"
#include "flare/block_reader.hpp"
#include "flare/block_types.hpp"
#include "flare/custom_property.hpp"
#include "flare/dependency_sorter.hpp"
#include "flare/exceptions/missing_file_exception.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include "flare/jelly_component.hpp"
#include "flare/paint/actor_color_fill.hpp"
#include "flare/paint/actor_color_stroke.hpp"
#include "flare/paint/actor_gradient_fill.hpp"
#include "flare/paint/actor_gradient_stroke.hpp"
#include "flare/paint/actor_radial_gradient_fill.hpp"
#include "flare/paint/actor_radial_gradient_stroke.hpp"
#include "flare/path/actor_ellipse.hpp"
#include "flare/path/actor_path.hpp"
#include "flare/path/actor_polygon.hpp"
#include "flare/path/actor_rectangle.hpp"
#include "flare/path/actor_star.hpp"
#include "flare/path/actor_triangle.hpp"
#include <algorithm>
#include <assert.h>
#include <stdio.h>

using namespace flare;

ActorArtboard::ActorArtboard(Actor* actor) :
    m_Actor(actor),
    m_Width(0.0f),
    m_Height(0.0f),
    m_ClipContents(false),
    m_Flags(ActorFlags::IsDrawOrderDirty),
    m_ComponentCount(0),
    m_NodeCount(0),
    m_DirtDepth(0),
    m_Components(nullptr),
    m_Nodes(nullptr),
    m_Root(actor->makeNode()),
    m_EventCallbackUserData(nullptr),
    m_EventCallback(nullptr),
    m_ImageNodeCount(0),
    m_DrawableCount(0),
    m_AnimationsCount(0),
    m_NestedActorAssetCount(0),
    m_NestedActorNodeCount(0),
    m_ImageNodes(nullptr),
    m_Drawables(nullptr),
    m_Animations(nullptr),
    m_NestedActorAssets(nullptr),
    m_NestedActorNodes(nullptr)
{
}

ActorArtboard::~ActorArtboard() { dispose(); }

void ActorArtboard::dispose()
{
	for (unsigned int i = 0; i < m_ComponentCount; i++)
	{
		// N.B. m_Root gets deleted here too as it is at the 0th index.
		delete m_Components[i];
	}
	for (unsigned int i = 0; i < m_DrawableCount; i++)
	{
		delete m_Drawables[i];
	}
	delete[] m_Components;
	delete[] m_Nodes;
	delete[] m_ImageNodes;
	delete[] m_NestedActorNodes;
	delete[] m_Drawables;

	if ((m_Flags & ActorFlags::IsInstance) != ActorFlags::IsInstance)
	{
		delete[] m_Animations;

		for (int i = 0; i < m_NestedActorAssetCount; i++)
		{
			delete m_NestedActorAssets[i];
		}
		delete[] m_NestedActorAssets;
	}

	m_ComponentCount = 0;
	m_NodeCount = 0;
	m_ImageNodeCount = 0;
	m_DrawableCount = 0;
	m_AnimationsCount = 0;
	m_NestedActorAssetCount = 0;
	m_Components = nullptr;
	m_Nodes = nullptr;
	m_ImageNodes = nullptr;
	m_Drawables = nullptr;
	m_Animations = nullptr;
	m_NestedActorAssets = nullptr;
	m_Root = nullptr;
	m_NestedActorNodes = nullptr;
}

ActorNode* ActorArtboard::root() const { return m_Root; }
Actor* ActorArtboard::actor() const { return m_Actor; }

ActorComponent* ActorArtboard::component(unsigned int index) const { return m_Components[index]; }

ActorComponent* ActorArtboard::component(unsigned short index) const { return m_Components[index]; }

ActorComponent* ActorArtboard::component(const std::string& name) const
{
	for (unsigned int i = 0; i < m_ComponentCount; i++)
	{
		ActorComponent* a = m_Components[i];
		if (a->name() == name)
		{
			return a;
		}
	}
	return nullptr;
}

NestedActorAsset* ActorArtboard::nestedActorAsset(unsigned int index) const
{
	if (index < m_NestedActorAssetCount)
	{
		return m_NestedActorAssets[index];
	}
	return nullptr;
}

void ActorArtboard::sortDependencies()
{
	DependencySorter sorter;
	sorter.sort(m_Root, m_DependencyOrder);
	unsigned int graphOrder = 0;
	for (unsigned int i = 0; i < m_ComponentCount; i++)
	{
		ActorComponent* component = m_Components[i];
		if (component == nullptr)
		{
			continue;
		}
		component->m_GraphOrder = graphOrder++;
		component->m_DirtMask = DirtyFlags::Filty;
	}
	m_Flags |= ActorFlags::IsDirty;
}

bool ActorArtboard::addDependency(ActorComponent* a, ActorComponent* b)
{
	auto& dependents = b->dependents();
	if (std::find(dependents.begin(), dependents.end(), a) != dependents.end())
	{
		// Already contained.
		return false;
	}

	dependents.push_back(a);
	return true;
}

void ActorArtboard::eventCallback(ActorAnimationEvent::Callback callback, void* userdata)
{
	m_EventCallbackUserData = userdata;
	m_EventCallback = callback;
}

ActorAnimation* ActorArtboard::animation(const std::string& name) const
{
	for (int i = 0; i < m_AnimationsCount; i++)
	{
		ActorAnimation& a = m_Animations[i];
		if (a.name() == name)
		{
			return &a;
		}
	}
	return nullptr;
}

ActorAnimationInstance* ActorArtboard::animationInstance(const std::string& name)
{
	ActorAnimation* a = animation(name);
	if (a == nullptr)
	{
		return nullptr;
	}
	return new ActorAnimationInstance(this, a);
}

void ActorArtboard::read(BlockReader* reader)
{
	m_Name = reader->readString();
	reader->read(m_Translation);
	m_Width = reader->readFloat32();
	m_Height = reader->readFloat32();
	reader->read(m_Origin);
	m_ClipContents = reader->readUint8() == 1;
	reader->read(m_Color);

	BlockReader* block = nullptr;
	while ((block = reader->readNextBlock()) != nullptr)
	{
		switch (block->blockType<BlockType>())
		{
			case BlockType::Components:
				readComponentsBlock(block);
				break;
			case BlockType::Animations:
				readAnimationsBlock(block);
				break;
			case BlockType::NestedActorAssets:
				readNestedActorAssetsBlock(block);
				break;
			default:
				break;
		}
		block->close();
	}
}

void ActorArtboard::readNestedActorAssetsBlock(BlockReader* block)
{
	m_NestedActorAssetCount = (int)block->readUint16();
	m_NestedActorAssets = new NestedActorAsset*[m_NestedActorAssetCount];

	BlockReader* nestedActorAssetBlock = nullptr;
	int nestedActorIndex = 0;

	while ((nestedActorAssetBlock = block->readNextBlock()) != nullptr)
	{
		switch (nestedActorAssetBlock->blockType<BlockType>())
		{
			case BlockType::NestedActorAsset:
				// Sanity check.
				if (nestedActorIndex < m_NestedActorAssetCount)
				{
					NestedActorAsset* asset = m_Actor->makeNestedActorAsset();
					asset->read(nestedActorAssetBlock);
					m_NestedActorAssets[nestedActorIndex++] = asset;
				}
				break;
			default:
				break;
		}

		nestedActorAssetBlock->close();
	};
}

void ActorArtboard::readAnimationsBlock(BlockReader* block)
{
	m_AnimationsCount = (int)block->readUint16();
	m_Animations = new ActorAnimation[m_AnimationsCount];

	BlockReader* animationBlock = nullptr;
	int animationIndex = 0;

	while ((animationBlock = block->readNextBlock()) != nullptr)
	{
		switch (animationBlock->blockType<BlockType>())
		{
			case BlockType::Animation:
				// Sanity check.
				if (animationIndex < m_AnimationsCount)
				{
					m_Animations[animationIndex++].read(animationBlock, m_Components, m_ComponentCount);
				}
				break;
			default:
				break;
		}

		animationBlock->close();
	};
}

void ActorArtboard::readComponentsBlock(BlockReader* block)
{
	m_ComponentCount = (unsigned int)(block->readUint16() + 1);
	m_Components = new ActorComponent*[m_ComponentCount];
	m_Components[0] = m_Root;

	BlockReader* componentBlock = nullptr;
	int componentIndex = 1;
	m_NodeCount = 1;
	while ((componentBlock = block->readNextBlock()) != nullptr)
	{
		ActorComponent* component = nullptr;
		switch (componentBlock->blockType<BlockType>())
		{
			case BlockType::ActorNode:
				component = ActorNode::read(this, componentBlock, m_Actor->makeNode());
				break;
			case BlockType::ActorBone:
				component = ActorBone::read(this, componentBlock);
				break;
			case BlockType::ActorRootBone:
				component = ActorRootBone::read(this, componentBlock);
				break;
			case BlockType::ActorImage:
			{
				m_DrawableCount++;
				m_ImageNodeCount++;
				component = ActorImage::read(this, componentBlock, m_Actor->makeImageNode());
				ActorImage* imageNode = static_cast<ActorImage*>(component);
				if (imageNode->textureIndex() > m_Actor->m_MaxTextureIndex)
				{
					m_Actor->m_MaxTextureIndex = imageNode->textureIndex();
				}
				break;
			}
			case BlockType::ActorImageSequence:
			{
				m_DrawableCount++;
				m_ImageNodeCount++;
				component = ActorImage::readImageSequence(this, componentBlock, m_Actor->makeImageNode());
				ActorImage* imageNode = static_cast<ActorImage*>(component);
				SequenceFrame* frames = imageNode->sequenceFrames();
				int index = frames[imageNode->sequenceFramesCount() - 1].m_AtlasIndex; // Last SequenceFrame has the biggest texture index
				if (index > m_Actor->m_MaxTextureIndex)
				{
					m_Actor->m_MaxTextureIndex = index;
				}
				break;
			}
			case BlockType::ActorStaticMesh:
			{
				m_DrawableCount++;
				component = ActorStaticMesh::read(this, componentBlock, m_Actor->makeStaticMeshNode());

				ActorStaticMesh* staticMeshNode = static_cast<ActorStaticMesh*>(component);
				for (int i = 0; i < staticMeshNode->numSurfaces(); i++)
				{
					ActorStaticMesh::Surface* surface = staticMeshNode->surface(i);
					if (surface->textureIndex > m_Actor->m_MaxTextureIndex)
					{
						m_Actor->m_MaxTextureIndex = surface->textureIndex;
					}
				}
				break;
			}
			case BlockType::NestedActorNode:
			{
				m_DrawableCount++;
				m_NestedActorNodeCount++;
				component = NestedActorNode::read(this, componentBlock, m_Actor->makeNestedActorNode());
				break;
			}
			case BlockType::ActorEvent:
				component = ActorEvent::read(this, componentBlock);
				break;
			case BlockType::CustomIntProperty:
				component = CustomIntProperty::read(this, componentBlock);
				break;
			case BlockType::CustomFloatProperty:
				component = CustomFloatProperty::read(this, componentBlock);
				break;
			case BlockType::CustomStringProperty:
				component = CustomStringProperty::read(this, componentBlock);
				break;
			case BlockType::CustomBooleanProperty:
				component = CustomBooleanProperty::read(this, componentBlock);
				break;
			case BlockType::ColliderRectangle:
				component = ActorColliderRectangle::read(this, componentBlock);
				break;
			case BlockType::ColliderTriangle:
				component = ActorColliderTriangle::read(this, componentBlock);
				break;
			case BlockType::ColliderCircle:
				component = ActorColliderCircle::read(this, componentBlock);
				break;
			case BlockType::ColliderPolygon:
				component = ActorColliderPolygon::read(this, componentBlock);
				break;
			case BlockType::ColliderLine:
				component = ActorColliderLine::read(this, componentBlock);
				break;
			case BlockType::ActorNodeSolo:
				component = ActorNodeSolo::read(this, componentBlock);
				break;
			case BlockType::ActorIKConstraint:
				component = ActorIKConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorScaleConstraint:
				component = ActorScaleConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorTranslationConstraint:
				component = ActorTranslationConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorRotationConstraint:
				component = ActorRotationConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorTransformConstraint:
				component = ActorTransformConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorDistanceConstraint:
				component = ActorDistanceConstraint::read(this, componentBlock);
				break;
			case BlockType::ActorJellyBone:
				component = ActorJellyBone::read(this, componentBlock);
				break;
			case BlockType::JellyComponent:
				component = JellyComponent::read(this, componentBlock);
				break;
			case BlockType::ActorSkin:
				component = ActorComponent::read(this, componentBlock, new ActorSkin());
				break;
			case BlockType::ActorShape:
				m_DrawableCount++;
				component = ActorDrawable::read(this, componentBlock, m_Actor->makeShape());
				break;
			case BlockType::ActorEllipse:
				component = ActorProceduralPath::read(this, componentBlock, m_Actor->makeEllipse());
				break;
			case BlockType::ActorRectangle:
				component = ActorRectangle::read(this, componentBlock, m_Actor->makeRectangle());
				break;
			case BlockType::ActorTriangle:
				component = ActorProceduralPath::read(this, componentBlock, m_Actor->makeTriangle());
				break;
			case BlockType::ActorStar:
				component = ActorStar::read(this, componentBlock, m_Actor->makeStar());
				break;
			case BlockType::ActorPolygon:
				component = ActorPolygon::read(this, componentBlock, m_Actor->makePolygon());
				break;
			case BlockType::ActorPath:
				component = ActorPath::read(this, componentBlock, m_Actor->makePath());
				break;
			case BlockType::ColorFill:
				component = ActorColorFill::read(this, componentBlock, m_Actor->makeColorFill());
				break;
			case BlockType::ColorStroke:
				component = ActorColorStroke::read(this, componentBlock, m_Actor->makeColorStroke());
				break;
			case BlockType::GradientFill:
				component = ActorGradientFill::read(this, componentBlock, m_Actor->makeGradientFill());
				break;
			case BlockType::GradientStroke:
				component = ActorGradientStroke::read(this, componentBlock, m_Actor->makeGradientStroke());
				break;
			case BlockType::RadialGradientFill:
				component = ActorRadialGradientFill::read(this, componentBlock, m_Actor->makeRadialGradientFill());
				break;
			case BlockType::RadialGradientStroke:
				component = ActorRadialGradientStroke::read(this, componentBlock, m_Actor->makeRadialGradientStroke());
				break;
			default:
				// Not handled/expected block.
				break;
		}
		if (component != nullptr && component->isNode())
		{
			m_NodeCount++;
		}
		m_Components[componentIndex] = component;
		componentIndex++;

		componentBlock->close();
	}

	m_Nodes = new ActorNode*[m_NodeCount];
	m_ImageNodes = new ActorImage*[m_ImageNodeCount];
	m_Drawables = new ActorDrawableProxy*[m_DrawableCount];
	m_NestedActorNodes = new NestedActorNode*[m_NestedActorNodeCount];
	m_Nodes[0] = m_Root;

	// Resolve nodes.
	int imdIdx = 0;
	int rndIdx = 0;
	int ndeIdx = 1;
	int nanIdx = 0;

	for (unsigned int i = 1; i < m_ComponentCount; i++)
	{
		ActorComponent* component = m_Components[i];
		if (component != nullptr)
		{
			component->resolveComponentIndices(m_Components, m_ComponentCount);

			switch (component->type())
			{
				case ComponentType::NestedActorNode:
					m_NestedActorNodes[nanIdx++] = static_cast<NestedActorNode*>(component);
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<NestedActorNode*>(component));
					break;
				case ComponentType::ActorStaticMesh:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorStaticMesh*>(component));
					break;
				case ComponentType::ActorImage:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorImage*>(component));
					m_ImageNodes[imdIdx++] = static_cast<ActorImage*>(component);
					break;
				case ComponentType::ActorShape:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorShape*>(component));
					break;
				default:
					break;
			}

			if (component->isNode())
			{
				m_Nodes[ndeIdx++] = static_cast<ActorNode*>(component);
			}
		}
	}

	for (unsigned int i = 1; i < m_ComponentCount; i++)
	{
		ActorComponent* component = m_Components[i];
		if (component != nullptr)
		{
			component->completeResolve();
		}
	}

	sortDependencies();
}

static bool DrawOrderComparer(ActorDrawableProxy* a, ActorDrawableProxy* b)
{
	return a->drawable()->drawOrder() < b->drawable()->drawOrder();
}

void ActorArtboard::copy(const ActorArtboard& artboard)
{
	m_Name = artboard.m_Name;
	Vec2D::copy(m_Translation, artboard.m_Translation);
	m_Width = artboard.m_Width;
	m_Height = artboard.m_Height;
	Vec2D::copy(m_Origin, artboard.m_Origin);
	m_ClipContents = artboard.m_ClipContents;
	m_Color.copy(artboard.m_Color);

	m_Flags = artboard.m_Flags;
	m_Flags |= ActorFlags::IsInstance;
	m_Animations = artboard.m_Animations;
	m_AnimationsCount = artboard.m_AnimationsCount;
	m_ImageNodeCount = artboard.m_ImageNodeCount;
	m_DrawableCount = artboard.m_DrawableCount;
	m_ComponentCount = artboard.m_ComponentCount;
	m_NodeCount = artboard.m_NodeCount;
	m_NestedActorAssetCount = artboard.m_NestedActorAssetCount;
	m_NestedActorNodeCount = artboard.m_NestedActorNodeCount;
	m_NestedActorAssets = artboard.m_NestedActorAssets;

	if (m_ComponentCount != 0)
	{
		m_Components = new ActorComponent*[m_ComponentCount];
	}
	if (m_NodeCount != 0)
	{
		m_Nodes = new ActorNode*[m_NodeCount];
	}
	if (m_DrawableCount != 0)
	{
		m_Drawables = new ActorDrawableProxy*[m_DrawableCount];
	}
	if (m_ImageNodeCount != 0)
	{
		m_ImageNodes = new ActorImage*[m_ImageNodeCount];
	}
	if (m_NestedActorNodeCount != 0)
	{
		m_NestedActorNodes = new NestedActorNode*[m_NestedActorNodeCount];
	}

	if (m_ComponentCount > 0)
	{
		int idx = 0;
		int rndIdx = 0;
		int imgIdx = 0;
		int ndeIdx = 0;
		int nanIdx = 0;

		for (int i = 0; i < m_ComponentCount; i++)
		{
			ActorComponent* component = artboard.m_Components[i];
			if (component == nullptr)
			{
				m_Components[idx++] = nullptr;
				continue;
			}
			ActorComponent* instanceComponent = component->makeInstance(this);
			m_Components[idx++] = instanceComponent;
			switch (instanceComponent->type())
			{
				case ComponentType::NestedActorNode:
					m_NestedActorNodes[nanIdx++] = static_cast<NestedActorNode*>(instanceComponent);
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<NestedActorNode*>(instanceComponent));
					break;
				case ComponentType::ActorStaticMesh:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorStaticMesh*>(instanceComponent));
					break;
				case ComponentType::ActorImage:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorImage*>(instanceComponent));
					m_ImageNodes[imgIdx++] = static_cast<ActorImage*>(instanceComponent);
					break;
				case ComponentType::ActorShape:
					m_Drawables[rndIdx++] = m_Actor->makeDrawableProxy(static_cast<ActorShape*>(instanceComponent));
					break;
				default:
					break;
			}

			if (instanceComponent->isNode())
			{
				m_Nodes[ndeIdx++] = static_cast<ActorNode*>(instanceComponent);
			}
		}

		// Resolve indices.
		m_Root = m_Nodes[0];
		for (int i = 1; i < m_ComponentCount; i++)
		{
			ActorComponent* component = m_Components[i];
			if (component == nullptr)
			{
				continue;
			}
			component->resolveComponentIndices(m_Components, m_ComponentCount);
		}

		for (unsigned int i = 1; i < m_ComponentCount; i++)
		{
			ActorComponent* component = m_Components[i];
			if (component == nullptr)
			{
				continue;
			}
			component->completeResolve();
		}
	}

	sortDependencies();
}

const int ActorArtboard::nestedActorCount() const { return m_NestedActorAssetCount; }

void ActorArtboard::markDrawOrderDirty() { m_Flags |= ActorFlags::IsDrawOrderDirty; }

int ActorArtboard::componentIndex(const ActorComponent* component) const
{
	for (int i = 0; i < m_ComponentCount; i++)
	{
		if (m_Components[i] == component)
		{
			return i;
		}
	}
	return -1;
}

bool ActorArtboard::addDirt(ActorComponent* component, DirtyFlags value, bool recurse)
{
	if ((component->m_DirtMask & value) == value)
	{
		// Already marked.
		return false;
	}

	// Make sure dirt is set before calling anything that can set more dirt.
	DirtyFlags dirt = component->m_DirtMask | value;
	component->m_DirtMask = dirt;

	m_Flags |= ActorFlags::IsDirty;

	component->onDirty(dirt);

	// If the order of this component is less than the current dirt depth, update the dirt depth
	// so that the update loop can break out early and re-run (something up the tree is dirty).
	if (component->m_GraphOrder < m_DirtDepth)
	{
		m_DirtDepth = component->m_GraphOrder;
	}

	if (!recurse)
	{
		return true;
	}

	auto dependents = component->dependents();
	for (auto dependent : dependents)
	{
		addDirt(dependent, value, true);
	}

	return true;
}

void ActorArtboard::update()
{
	if ((m_Flags & ActorFlags::IsDirty) == ActorFlags::IsDirty)
	{
		const int maxSteps = 100;
		int step = 0;
		int count = m_DependencyOrder.size();
		while ((m_Flags & ActorFlags::IsDirty) == ActorFlags::IsDirty && step < maxSteps)
		{
			m_Flags &= ~ActorFlags::IsDirty;
			// Track dirt depth here so that if something else marks dirty, we restart.
			for (int i = 0; i < count; i++)
			{
				ActorComponent* component = m_DependencyOrder[i];
				m_DirtDepth = (unsigned int)i;
				DirtyFlags d = component->m_DirtMask;
				if (d == DirtyFlags::None)
				{
					continue;
				}
				component->m_DirtMask = DirtyFlags::None;
				component->update(d);
				if (m_DirtDepth < i)
				{
					break;
				}
			}
			step++;
		}

		// Todo: Don't we want to always reset dirt depth to 0 here?
		// m_DirtDepth = 0;
	}

	if ((m_Flags & ActorFlags::IsDrawOrderDirty) == ActorFlags::IsDrawOrderDirty)
	{
		m_Flags &= ~ActorFlags::IsDrawOrderDirty;

		if (m_Drawables != nullptr)
		{
			std::sort(m_Drawables, m_Drawables + m_DrawableCount, DrawOrderComparer);
		}
	}
}

void ActorArtboard::advance(float elapsedSeconds)
{
	update();

	// Advance nested.
	for (unsigned int i = 0; i < m_NestedActorNodeCount; i++)
	{
		m_NestedActorNodes[i]->advance(elapsedSeconds);
	}
}

ActorArtboard* ActorArtboard::makeInstance(Actor* resetActor) const
{
	ActorArtboard* instanceArtboard = new ActorArtboard(resetActor);
	instanceArtboard->copy(*this);
	return instanceArtboard;
}

unsigned int ActorArtboard::componentCount() const { return m_ComponentCount; }

AABB ActorArtboard::aabb() const
{
	float minX = -m_Origin[0] * m_Width;
	float minY = -m_Origin[1] * m_Height;
	return AABB(minX, minY, minX + m_Width, minY + m_Height);
}