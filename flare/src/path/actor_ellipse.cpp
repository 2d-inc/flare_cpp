#include "flare/path/actor_ellipse.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

const float CircleConstant = 0.55f;

ActorEllipse::ActorEllipse() : Base(ComponentType::ActorEllipse) {}

void ActorEllipse::makePoints()
{
	float radiusX = width() / 2.0f;
	float radiusY = height() / 2.0f;
	
    m_Points.push_back(new CubicPathPoint(
        Vec2D(0.0f, -radiusY),
        Vec2D(-radiusX * CircleConstant, -radiusY),
        Vec2D(radiusX * CircleConstant, -radiusY)));
    m_Points.push_back(new CubicPathPoint(
        Vec2D(radiusX, 0.0f),
        Vec2D(radiusX, CircleConstant * -radiusY),
        Vec2D(radiusX, CircleConstant * radiusY)));
    m_Points.push_back(new CubicPathPoint(
        Vec2D(0.0f, radiusY),
        Vec2D(radiusX * CircleConstant, radiusY),
        Vec2D(-radiusX * CircleConstant, radiusY)));
    m_Points.push_back(new CubicPathPoint(
        Vec2D(-radiusX, 0.0f),
        Vec2D(-radiusX, radiusY * CircleConstant),
        Vec2D(-radiusX, -radiusY * CircleConstant)));
}

ActorComponent* ActorEllipse::makeInstance(ActorArtboard* artboard) const
{
	ActorEllipse* instance = new ActorEllipse();
	instance->copy(this, artboard);
	return instance;
}