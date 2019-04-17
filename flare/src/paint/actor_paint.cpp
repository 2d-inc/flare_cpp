#include "flare/paint/actor_paint.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorPaint::ActorPaint(ComponentType type) :
    ActorComponent(nullptr, type),
    m_Opacity(1.0f),
    m_RenderOpacity(1.0f),
    m_IsPaintInvalid(false),
    m_Shape(nullptr)
{
}

void ActorPaint::onParentChanged(ActorNode* from, ActorNode* to)
{
	if (to != nullptr && to->type() == ComponentType::ActorShape)
	{
		ActorShape* oldShape = m_Shape;
		m_Shape = static_cast<ActorShape*>(to);
		onShapeChanged(oldShape, m_Shape);
	}
}

void ActorPaint::opacity(float value)
{
	if (value == m_Opacity)
	{
		return;
	}
	m_Opacity = value;
	markPaintDirty();
}

void ActorPaint::copy(const ActorPaint* paint, ActorArtboard* artboard)
{
	Base::copy(paint, artboard);
	m_Opacity = paint->m_Opacity;
}

ActorPaint* ActorPaint::read(ActorArtboard* artboard, BlockReader* reader, ActorPaint* component)
{
	Base::read(artboard, reader, component);
	component->m_Opacity = reader->readFloat32();
	return component;
}

void ActorPaint::completeResolve()
{
	Base::completeResolve();
	m_Artboard->addDependency(this, m_Parent);
}

void ActorPaint::update(DirtyFlags dirt)
{
	float ro = m_Shape != nullptr ? m_Shape->renderOpacity() * m_Opacity : 1.0f;
	if (ro != m_RenderOpacity)
	{
		m_RenderOpacity = ro;
		invalidatePaint();
	}

	// If something has forced our dirty flag up, make sure we invalidate the paint.
	if((dirt & DirtyFlags::PaintDirty) == DirtyFlags::PaintDirty)
	{
		invalidatePaint();
	}
}

void ActorPaint::markPaintDirty() { m_Artboard->addDirt(this, DirtyFlags::PaintDirty, false); }

void ActorPaint::invalidatePaint() { m_IsPaintInvalid = true; }

void ActorPaint::validatePaint()
{
	if(m_IsPaintInvalid)
	{
		m_IsPaintInvalid = false;
		updatePaint();
	}
}