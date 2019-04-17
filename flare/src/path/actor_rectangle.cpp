#include "flare/path/actor_rectangle.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorRectangle::ActorRectangle() : Base(ComponentType::ActorRectangle), m_Radius(0.0f) {}

void ActorRectangle::copy(const ActorRectangle* rectangle, ActorArtboard* artboard)
{
	Base::copy(rectangle, artboard);
	m_Radius = rectangle->m_Radius;
}

void ActorRectangle::makePoints()
{
	float halfWidth = width() / 2.0f;
	float halfHeight = height() / 2.0f;

	m_Points.push_back(new StraightPathPoint(Vec2D(-halfWidth, -halfHeight), m_Radius));
	m_Points.push_back(new StraightPathPoint(Vec2D(halfWidth, -halfHeight), m_Radius));
	m_Points.push_back(new StraightPathPoint(Vec2D(halfWidth, halfHeight), m_Radius));
	m_Points.push_back(new StraightPathPoint(Vec2D(-halfWidth, halfHeight), m_Radius));
}

ActorComponent* ActorRectangle::makeInstance(ActorArtboard* artboard) const
{
	ActorRectangle* instance = new ActorRectangle();
	instance->copy(this, artboard);
	return instance;
}

ActorRectangle* ActorRectangle::read(ActorArtboard* artboard, BlockReader* reader, ActorRectangle* component)
{
	Base::read(artboard, reader, component);
	component->m_Radius = reader->readFloat32();
	return component;
}

bool ActorRectangle::radius(float value)
{
	if(m_Radius == value)
	{
		return false;
	}
	invalidateDrawable();
	return true;
}