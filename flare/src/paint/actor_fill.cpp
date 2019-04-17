#include "flare/paint/actor_fill.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorFill::ActorFill() : m_FillRule(FillRule::EvenOdd) {}

void ActorFill::copy(const ActorFill* fill, ActorArtboard* artboard)
{
	m_FillRule = fill->m_FillRule;
}

ActorFill* ActorFill::read(ActorArtboard* artboard, BlockReader* reader, ActorFill* component)
{
    component->m_FillRule = (FillRule)reader->readUint8();
	return component;
}