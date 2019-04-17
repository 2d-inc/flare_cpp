#include "flare/custom_property.hpp"
#include "flare/block_reader.hpp"
using namespace flare;

CustomIntProperty::CustomIntProperty() : ActorComponent(ComponentType::CustomIntProperty), m_Value(0) {}

ActorComponent* CustomIntProperty::makeInstance(ActorArtboard* artboard) const
{
	CustomIntProperty* instanceProp = new CustomIntProperty();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void CustomIntProperty::copy(const CustomIntProperty* property, ActorArtboard* artboard)
{
	Base::copy(property, artboard);
	m_Value = property->m_Value;
}

CustomIntProperty* CustomIntProperty::read(ActorArtboard* artboard, BlockReader* reader, CustomIntProperty* property)
{
	if (property == nullptr)
	{
		property = new CustomIntProperty();
	}
	ActorComponent::read(artboard, reader, property);
	property->m_Value = reader->readInt32();
	return property;
}

void CustomIntProperty::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);

	ActorComponent* parent = components[parentIdx()];
	if (parent != nullptr)
	{
		parent->addCustomIntProperty(this);
	}
}

int CustomIntProperty::value() const { return m_Value; }

void CustomIntProperty::value(int v) { m_Value = v; }

CustomFloatProperty::CustomFloatProperty() : ActorComponent(ComponentType::CustomFloatProperty), m_Value(0.0f) {}

ActorComponent* CustomFloatProperty::makeInstance(ActorArtboard* artboard) const
{
	CustomFloatProperty* instanceProp = new CustomFloatProperty();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void CustomFloatProperty::copy(const CustomFloatProperty* property, ActorArtboard* artboard)
{
	Base::copy(property, artboard);
	m_Value = property->m_Value;
}

CustomFloatProperty* CustomFloatProperty::read(ActorArtboard* artboard, BlockReader* reader, CustomFloatProperty* property)
{
	if (property == nullptr)
	{
		property = new CustomFloatProperty();
	}
	ActorComponent::read(artboard, reader, property);
	property->m_Value = reader->readFloat32();
	return property;
}

void CustomFloatProperty::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	ActorComponent* parent = components[parentIdx()];
	if (parent != nullptr)
	{
		parent->addCustomFloatProperty(this);
	}
}

float CustomFloatProperty::value() const { return m_Value; }

void CustomFloatProperty::value(float v) { m_Value = v; }

CustomStringProperty::CustomStringProperty() : ActorComponent(ComponentType::CustomStringProperty) {}

ActorComponent* CustomStringProperty::makeInstance(ActorArtboard* artboard) const
{
	CustomStringProperty* instanceProp = new CustomStringProperty();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void CustomStringProperty::copy(const CustomStringProperty* property, ActorArtboard* artboard)
{
	Base::copy(property, artboard);
	m_Value = property->m_Value;
}

CustomStringProperty* CustomStringProperty::read(ActorArtboard* artboard, BlockReader* reader, CustomStringProperty* property)
{
	if (property == nullptr)
	{
		property = new CustomStringProperty();
	}
	ActorComponent::read(artboard, reader, property);
	property->m_Value = reader->readString();
	return property;
}

void CustomStringProperty::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);

	ActorComponent* parent = components[parentIdx()];
	if (parent != nullptr)
	{
		parent->addCustomStringProperty(this);
	}
}

const std::string& CustomStringProperty::value() const { return m_Value; }

void CustomStringProperty::value(const std::string& v) { m_Value = v; }

CustomBooleanProperty::CustomBooleanProperty() : ActorComponent(ComponentType::CustomBooleanProperty) {}

ActorComponent* CustomBooleanProperty::makeInstance(ActorArtboard* artboard) const
{
	CustomBooleanProperty* instanceProp = new CustomBooleanProperty();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void CustomBooleanProperty::copy(const CustomBooleanProperty* property, ActorArtboard* artboard)
{
	Base::copy(property, artboard);
	m_Value = property->m_Value;
}

CustomBooleanProperty* CustomBooleanProperty::read(ActorArtboard* artboard, BlockReader* reader, CustomBooleanProperty* property)
{
	if (property == nullptr)
	{
		property = new CustomBooleanProperty();
	}
	ActorComponent::read(artboard, reader, property);
	property->m_Value = reader->readUint8() == 1;
	return property;
}

void CustomBooleanProperty::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);

	ActorComponent* parent = components[parentIdx()];
	if (parent != nullptr)
	{
		parent->addCustomBooleanProperty(this);
	}
}

const bool CustomBooleanProperty::value() const { return m_Value; }

void CustomBooleanProperty::value(const bool v) { m_Value = v; }