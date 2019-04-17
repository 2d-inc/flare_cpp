#include "flare/paint/actor_gradient_color.hpp"
#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorGradientColor::ActorGradientColor(ComponentType type) : Base(type) {}

void ActorGradientColor::copy(const ActorGradientColor* fill, ActorArtboard* artboard)
{
	Base::copy(fill, artboard);
	m_ColorStops = std::vector<float>(fill->m_ColorStops);
	Vec2D::copy(m_Start, fill->m_Start);
	Vec2D::copy(m_End, fill->m_End);
	Vec2D::copy(m_RenderStart, fill->m_RenderStart);
	Vec2D::copy(m_RenderEnd, fill->m_RenderEnd);
}

ActorGradientColor* ActorGradientColor::read(ActorArtboard* artboard, BlockReader* reader,
                                             ActorGradientColor* component)
{
	Base::read(artboard, reader, component);
	int numStops = reader->readUint8();
	int stopsLength = numStops * 5;
	component->m_ColorStops.resize(stopsLength);
	reader->readFloat32Array(&component->m_ColorStops[0], stopsLength);
	reader->read(component->m_Start);
	reader->read(component->m_End);
	return component;
}

void ActorGradientColor::update(DirtyFlags dirt)
{
	Base::update(dirt);
	if ((dirt & DirtyFlags::WorldTransformDirty) == DirtyFlags::WorldTransformDirty && m_Shape != nullptr)
	{
		Vec2D temp;
		const Mat2D world = m_Shape->worldTransform();
		Vec2D::transform(temp, m_Start, world);
		if (m_RenderStart != temp)
		{
			Vec2D::copy(m_RenderStart, temp);
			invalidatePaint();
		}
		Vec2D::transform(temp, m_End, world);
		if (m_RenderStart != temp)
		{
			Vec2D::copy(m_RenderStart, temp);
			invalidatePaint();
		}
	}
}

bool ActorGradientColor::start(const Vec2D& value)
{
	if(value == m_Start)
	{
		return false;
	}
	Vec2D::copy(m_Start, value);
	markPaintDirty();
	return true;
}

bool ActorGradientColor::end(const Vec2D& value)
{
	if(value == m_End)
	{
		return false;
	}
	Vec2D::copy(m_End, value);
	markPaintDirty();
	return true;
}

bool ActorGradientColor::colorStops(const std::vector<float>& value)
{
	if(value.size() != m_ColorStops.size())
	{
		return false;
	}
	m_ColorStops.assign(value.begin(), value.end());
	invalidatePaint();
	return true;
}