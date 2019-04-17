#include "flare/animation/actor_animation.hpp"
#include "flare/actor_component.hpp"
#include "flare/block_reader.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include <algorithm>

using namespace flare;

ActorAnimation::ActorAnimation() : m_FPS(60), m_Duration(0.0f), m_IsLooping(false) {}

ActorAnimation::~ActorAnimation()
{
	for (auto component : m_AnimatedComponents)
	{
		delete component;
	}
	for (auto component : m_TriggerComponents)
	{
		delete component;
	}
}

const std::string& ActorAnimation::name() const { return m_Name; }

void ActorAnimation::duration(const float d)
{
	if (m_Duration != d)
	{
		m_Duration = d;
	}
}

float ActorAnimation::duration() const { return m_Duration; }

bool ActorAnimation::isLooping() const { return m_IsLooping; }

int ActorAnimation::fps() const { return m_FPS; }

void ActorAnimation::fps(int v)
{
	if (m_FPS != v)
	{
		m_FPS = v;
	}
}

void ActorAnimation::apply(float time, ActorArtboard* artboard, float mix)
{
	for (auto component : m_AnimatedComponents)
	{
		component->apply(time, artboard, mix);
	}
}

void ActorAnimation::triggerEvents(ActorArtboard* artboard, float fromTime, float toTime,
                                   std::vector<ActorAnimationEvent>& events)
{
	for (auto keyedComponent : m_TriggerComponents)
	{
		keyedComponent->triggerEvents(artboard, fromTime, toTime, events);
	}
}

void ActorAnimation::read(BlockReader* reader, ActorComponent** components, int componentCount)
{
	m_Name = reader->readString();
	m_FPS = (int)reader->readUint8();
	m_Duration = reader->readFloat32();
	m_IsLooping = reader->readUint8() != 0;

	int count = (int)reader->readUint16();

	for (int i = 0; i < count; i++)
	{
		ComponentAnimation* componentAnimation = new ComponentAnimation();
		componentAnimation->read(reader, components);

		// Put actor events in a separate list...
		int componentIndex = componentAnimation->componentIndex();
		if (componentIndex >= componentCount)
		{
			throw OverflowException("Attempt to read past end of components in ActorAnimation::read.");
		}
		ActorComponent* component = components[componentAnimation->componentIndex()];
		if (component == nullptr)
		{
			delete componentAnimation;
		}
		else if (component->type() == ComponentType::ActorEvent)
		{
			m_TriggerComponents.emplace_back(componentAnimation);
		}
		else
		{
			m_AnimatedComponents.emplace_back(componentAnimation);
		}
	}
}