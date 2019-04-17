#include "flare/path/actor_triangle.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorTriangle::ActorTriangle() : Base(ComponentType::ActorTriangle) {}

void ActorTriangle::makePoints()
{
	float radiusX = width() / 2.0f;
	float radiusY = height() / 2.0f;

	m_Points.push_back(new StraightPathPoint(Vec2D(0.0f, -radiusY)));
	m_Points.push_back(new StraightPathPoint(Vec2D(radiusX, radiusY)));
	m_Points.push_back(new StraightPathPoint(Vec2D(-radiusX, radiusY)));
}

ActorComponent* ActorTriangle::makeInstance(ActorArtboard* artboard) const
{
	ActorTriangle* instance = new ActorTriangle();
	instance->copy(this, artboard);
	return instance;
}