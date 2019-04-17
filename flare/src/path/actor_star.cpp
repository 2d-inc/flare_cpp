#include "flare/path/actor_star.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorStar::ActorStar() : Base(ComponentType::ActorStar), m_NumPoints(5), m_InnerRadius(0.0f) {}

void ActorStar::makePoints()
{
	float radiusX = width() / 2.0f;
	float radiusY = height() / 2.0f;
	int numSides = m_NumPoints * 2;

	m_Points.push_back(new StraightPathPoint(Vec2D(0.0f, -radiusY)));

	float angle = -M_PI / 2.0;
	float inc = (M_PI * 2.0) / numSides;
	Vec2D sx(radiusX, radiusX * m_InnerRadius);
	Vec2D sy(radiusY, radiusY * m_InnerRadius);

	for (int i = 0; i < numSides; i++)
	{
		m_Points.push_back(new StraightPathPoint(Vec2D(cos(angle) * sx[i % 2], sin(angle) * sy[i % 2])));
		angle += inc;
	}
}

void ActorStar::copy(const ActorStar* star, ActorArtboard* artboard)
{
	Base::copy(star, artboard);
	m_NumPoints = star->m_NumPoints;
	m_InnerRadius = star->m_InnerRadius;
}

ActorComponent* ActorStar::makeInstance(ActorArtboard* artboard) const
{
	ActorStar* instance = new ActorStar();
	instance->copy(this, artboard);
	return instance;
}

ActorStar* ActorStar::read(ActorArtboard* artboard, BlockReader* reader, ActorStar* component)
{
	Base::read(artboard, reader, component);

	component->m_NumPoints = reader->readUint32();
	component->m_InnerRadius = reader->readFloat32();
	return component;
}

bool ActorStar::innerRadius(float value)
{
	if (value == m_InnerRadius)
	{
		return false;
	}
	m_InnerRadius = value;
	invalidateDrawable();
	return true;
}