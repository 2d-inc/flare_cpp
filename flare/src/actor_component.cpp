#include "flare/actor_component.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_node.hpp"
#include "flare/block_reader.hpp"
#include "flare/custom_property.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include <algorithm>
#include <cassert>
using namespace flare;

ActorComponent::ActorComponent(ComponentType type) : ActorComponent(nullptr, type) {}

ActorComponent::ActorComponent(ActorArtboard* artboard, ComponentType type) :
    m_Type(type),
    m_Parent(nullptr),
    m_Artboard(artboard),
    m_GraphOrder(0),
    m_DirtMask(DirtyFlags::None)
{
}

ActorComponent::~ActorComponent() {}

ActorArtboard* ActorComponent::artboard() const { return m_Artboard; }

const std::string& ActorComponent::name() const { return m_Name; }

ComponentType ActorComponent::type() const { return m_Type; }

ActorNode* ActorComponent::parent() const { return m_Parent; }
void ActorComponent::parent(ActorNode* value)
{
	if (m_Parent == value)
	{
		return;
	}
	ActorNode* lastParent = m_Parent;
	m_Parent = value;
	onParentChanged(lastParent, m_Parent);
}

unsigned short ActorComponent::parentIdx() const { return m_ParentIdx; }

int ActorComponent::componentIndex() const { return m_Artboard->componentIndex(this); }

void ActorComponent::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	if (m_ParentIdx >= numComponents)
	{
		throw OverflowException("Attempt to read past end of components in ActorComponent::resolveComponentIndices.");
	}
	ActorComponent* component = components[m_ParentIdx];
	if (component != nullptr)
	{
		m_Artboard->addDependency(this, component);
		if (component->isNode())
		{
			parent(static_cast<ActorNode*>(component));
			if (this->isNode())
			{
				m_Parent->addChild(static_cast<ActorNode*>(this));
			}
		}
	}
}

void ActorComponent::completeResolve() {}

void ActorComponent::copy(const ActorComponent* component, ActorArtboard* artboard)
{
	m_Name = component->m_Name;
	m_Artboard = artboard;
	m_ParentIdx = component->m_ParentIdx;
}

ActorComponent* ActorComponent::read(ActorArtboard* artboard, BlockReader* reader, ActorComponent* component)
{
	component->m_Artboard = artboard;
	component->m_Name = reader->readString();
	component->m_ParentIdx = reader->readUint16();

	return component;
}

void ActorComponent::addCustomIntProperty(CustomIntProperty* property) { m_CustomIntProperties.push_back(property); }

void ActorComponent::addCustomFloatProperty(CustomFloatProperty* property)
{
	m_CustomFloatProperties.push_back(property);
}

void ActorComponent::addCustomStringProperty(CustomStringProperty* property)
{
	m_CustomStringProperties.push_back(property);
}

void ActorComponent::addCustomBooleanProperty(CustomBooleanProperty* property)
{
	m_CustomBooleanProperties.push_back(property);
}

CustomIntProperty* ActorComponent::getCustomIntProperty(const std::string& name)
{
	for (CustomIntProperty* prop : m_CustomIntProperties)
	{
		if (prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomFloatProperty* ActorComponent::getCustomFloatProperty(const std::string& name)
{
	for (CustomFloatProperty* prop : m_CustomFloatProperties)
	{
		if (prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomStringProperty* ActorComponent::getCustomStringProperty(const std::string& name)
{
	for (CustomStringProperty* prop : m_CustomStringProperties)
	{
		if (prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}

CustomBooleanProperty* ActorComponent::getCustomBooleanProperty(const std::string& name)
{
	for (CustomBooleanProperty* prop : m_CustomBooleanProperties)
	{
		if (prop->name() == name)
		{
			return prop;
		}
	}
	return nullptr;
}