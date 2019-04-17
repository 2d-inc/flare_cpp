#include "flare/actor_shape.hpp"
#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

ActorShape::ActorShape() : ActorDrawable(ComponentType::ActorShape) {}

ActorShape::~ActorShape() {}

void ActorShape::update(DirtyFlags dirt)
{
	Base::update(dirt);
	invalidateDrawable();
}

void ActorShape::invalidateDrawable() {}

void ActorShape::addStroke(ActorStroke* stroke) { m_Strokes.push_back(stroke); onStrokesChanged(); }

void ActorShape::addFill(ActorFill* fill) { m_Fills.push_back(fill); onFillsChanged(); }

void ActorShape::removeStroke(ActorStroke* stroke)
{
	auto iterator = std::find(m_Strokes.begin(), m_Strokes.end(), stroke);
	if (iterator != m_Strokes.end())
	{
		m_Strokes.erase(iterator);
		 onStrokesChanged();
	}
}

void ActorShape::removeFill(ActorFill* fill)
{
	auto iterator = std::find(m_Fills.begin(), m_Fills.end(), fill);
	if (iterator != m_Fills.end())
	{
		m_Fills.erase(iterator);
		onFillsChanged();
	}
}

void ActorShape::initializeGraphics()
{
	for (ActorStroke* stroke : m_Strokes)
	{
		stroke->initializeGraphics();
	}
	for (ActorFill* fill : m_Fills)
	{
		fill->initializeGraphics();
	}
}
