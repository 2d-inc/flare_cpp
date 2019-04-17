#include "flare/animation/component_animation.hpp"
#include "flare/actor.hpp"
#include "flare/animation/actor_animation.hpp"
#include "flare/animation/keyframes/keyframe.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ComponentAnimation::ComponentAnimation() : m_ComponentIndex(0) {}

ComponentAnimation::~ComponentAnimation()
{
	for (auto property : m_Properties)
	{
		delete property;
	}
}

int ComponentAnimation::componentIndex() const { return m_ComponentIndex; }

void ComponentAnimation::apply(float time, ActorArtboard* artboard, float mix)
{
	ActorComponent* component = artboard->component(m_ComponentIndex);
	for (auto property : m_Properties)
	{
		property->apply(time, component, mix);
	}
}

void ComponentAnimation::triggerEvents(ActorArtboard* artboard, float fromTime, float toTime,
                                       std::vector<ActorAnimationEvent>& events)
{
	for (auto property : m_Properties)
	{
		switch (property->type())
		{
			case PropertyType::Trigger:
			{
				int keyFramesCount = property->keyFramesCount();
				if (keyFramesCount == 0)
				{
					continue;
				}
				int idx = 0;
				// Binary find the keyframe index.
				{
					int mid = 0;
					float element = 0.0f;
					int start = 0;
					int end = keyFramesCount - 1;

					while (start <= end)
					{
						mid = ((start + end) >> 1);
						element = property->keyFrame(mid)->time();
						if (element < toTime)
						{
							start = mid + 1;
						}
						else if (element > toTime)
						{
							end = mid - 1;
						}
						else
						{
							start = mid;
							break;
						}
					}

					idx = start;
				}

				if (idx == 0)
				{
					if (property->keyFrame(0)->time() == toTime)
					{
						ActorEvent* actorEvent = reinterpret_cast<ActorEvent*>(artboard->component(m_ComponentIndex));
						events.emplace_back(ActorAnimationEvent(actorEvent, toTime, 0.0f));
					}
				}
				else
				{
					for (int k = idx - 1; k >= 0; k--)
					{
						float frameTime = property->keyFrame(k)->time();

						if (frameTime > fromTime)
						{
							ActorEvent* actorEvent =
							    reinterpret_cast<ActorEvent*>(artboard->component(m_ComponentIndex));
							events.emplace_back(ActorAnimationEvent(actorEvent, frameTime, toTime - frameTime));
						}
						else
						{
							break;
						}
					}
				}
				break;
			}
			default:
				break;
		}
	}
}

void ComponentAnimation::read(BlockReader* reader, ActorComponent** components)
{
	m_ComponentIndex = reader->readUint16();
	int propertiesCount = (int)reader->readUint16();
	ActorComponent* component = components[m_ComponentIndex];
	for (int i = 0; i < propertiesCount; i++)
	{
		PropertyAnimation* propertyAnimation = new PropertyAnimation();
		propertyAnimation->read(reader, component);
		m_Properties.emplace_back(propertyAnimation);
	}
}