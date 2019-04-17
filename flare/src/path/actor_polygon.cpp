#include "flare/path/actor_polygon.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorPolygon::ActorPolygon() : Base(ComponentType::ActorPolygon), m_NumSides(5) {}

void ActorPolygon::makePoints()
{
	float radiusX = width() / 2.0f;
	float radiusY = height() / 2.0f;

    float angle = -M_PI / 2.0f;
    float inc = (M_PI * 2.0) / m_NumSides;

    for (int i = 0; i < m_NumSides; i++) {
      m_Points.push_back(new StraightPathPoint(
          Vec2D(cos(angle) * radiusX, sin(angle) * radiusY)));
      angle += inc;
    }
}

void ActorPolygon::numSides(int value)
{
    if(m_NumSides == value)
    {
        return;
    }
    m_NumSides = value;
    invalidateDrawable();
}

void ActorPolygon::copy(const ActorPolygon* star, ActorArtboard* artboard)
{
	Base::copy(star, artboard);
	m_NumSides = star->m_NumSides;
}

ActorComponent* ActorPolygon::makeInstance(ActorArtboard* artboard) const
{
	ActorPolygon* instance = new ActorPolygon();
	instance->copy(this, artboard);
	return instance;
}

ActorPolygon* ActorPolygon::read(ActorArtboard* artboard, BlockReader* reader, ActorPolygon* component)
{
	Base::read(artboard, reader, component);

	component->m_NumSides = reader->readUint32();
	return component;
}