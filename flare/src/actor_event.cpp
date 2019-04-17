#include "flare/actor_event.hpp"
#include "flare/block_reader.hpp"
using namespace flare;

ActorEvent::ActorEvent() : ActorComponent(ComponentType::ActorEvent) {}

ActorComponent* ActorEvent::makeInstance(ActorArtboard* artboard) const
{
	ActorEvent* eventInstance = new ActorEvent();
	eventInstance->copy(this, artboard);
	return eventInstance;
}

void ActorEvent::copy(const ActorComponent* node, ActorArtboard* artboard) { Base::copy(node, artboard); }

ActorEvent* ActorEvent::read(ActorArtboard* artboard, BlockReader* reader, ActorEvent* component)
{
	if (component == nullptr)
	{
		component = new ActorEvent();
	}

	Base::read(artboard, reader, component);

	return component;
}