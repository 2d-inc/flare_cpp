#include "flare/paint/actor_color.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorColor::ActorColor(ComponentType type) : Base(type) {}

void ActorColor::color(const Color& value)
{
	if (value == m_Color)
	{
		return;
	}
	m_Color = value;
	markPaintDirty();
}

void ActorColor::copy(const ActorColor* paint, ActorArtboard* artboard)
{
	Base::copy(paint, artboard);
	m_Color = paint->m_Color;
}

ActorColor* ActorColor::read(ActorArtboard* artboard, BlockReader* reader, ActorColor* component)
{
	Base::read(artboard, reader, component);
	reader->read(component->m_Color);
	return component;
}