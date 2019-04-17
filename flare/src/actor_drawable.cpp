#include "flare/actor_drawable.hpp"
#include "flare/actor.hpp"

using namespace flare;

ActorDrawable::ActorDrawable(ComponentType type) : ActorNode(type), m_DrawOrder(0) {}

void ActorDrawable::copy(const ActorDrawable* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	m_DrawOrder = node->m_DrawOrder;
	m_BlendMode = node->m_BlendMode;
	m_IsHidden = node->m_IsHidden;
}

int ActorDrawable::drawOrder() const { return m_DrawOrder; }

void ActorDrawable::drawOrder(int order)
{
	if (m_DrawOrder != order)
	{
		m_DrawOrder = order;
		if (m_Artboard != nullptr)
		{
			m_Artboard->markDrawOrderDirty();
		}
	}
}

bool ActorDrawable::isHidden() const { return m_IsHidden; }
void ActorDrawable::isHidden(bool value) { m_IsHidden = value; }

BlendMode ActorDrawable::blendMode() const { return m_BlendMode; }
void ActorDrawable::blendMode(BlendMode value)
{
	if (value == m_BlendMode)
	{
		return;
	}
	BlendMode old = m_BlendMode;
	m_BlendMode = value;
	onBlendModeChanged(old, m_BlendMode);
}

ActorDrawable* ActorDrawable::read(ActorArtboard* artboard, BlockReader* reader, ActorDrawable* component)
{
	Base::read(artboard, reader, component);

	component->m_IsHidden = reader->readUint8() == 0;
	component->m_BlendMode = (BlendMode)reader->readUint8();
	component->m_DrawOrder = (int)reader->readUint16();

	return component;
}

void ActorDrawable::onBlendModeChanged(BlendMode from, BlendMode to) {}