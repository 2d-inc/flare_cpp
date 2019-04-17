#include "flare/animation/actor_animation_instance.hpp"
#include "flare/actor.hpp"
#include <cmath>

using namespace flare;

ActorAnimationInstance::ActorAnimationInstance(ActorArtboard* artboard, ActorAnimation* animation) :
	m_Artboard(artboard),
	m_Animation(animation),
	m_Time(0.0f),
	m_Min(0.0f),
	m_Max(animation->duration()),
	m_Range(animation->duration()),
	m_Loop(animation->isLooping()),
	m_EventCallbackUserData(nullptr),
	m_EventCallback(nullptr)
{

}

ActorAnimationInstance::~ActorAnimationInstance()
{

}

float ActorAnimationInstance::duration() const
{
	return m_Range;
}

float ActorAnimationInstance::min() const
{
	return m_Min;
}

float ActorAnimationInstance::max() const
{
	return m_Max;
}

float ActorAnimationInstance::time() const
{
	return m_Time;
}

void ActorAnimationInstance::time(float value)
{
	float delta = value - m_Time;
	float time = m_Time + std::fmod(delta, m_Range);

	if(time < m_Min)
	{
		if(m_Loop)
		{
			time = m_Max - (m_Min - time);	
		}
		else
		{
			time = m_Min;
		}
	}
	else if(time > m_Max)
	{
		if(m_Loop)
		{
			time = m_Min + (time - m_Max);
		}
		else
		{
			time = m_Max;
		}
	}
	m_Time = time;
}

bool ActorAnimationInstance::isLooping() const
{
	return m_Loop;
}

void ActorAnimationInstance::isLooping(bool isIt)
{
	m_Loop = isIt;
}

void ActorAnimationInstance::eventCallback(ActorAnimationEvent::Callback callback, void* userdata)
{
	m_EventCallbackUserData = userdata;
	m_EventCallback = callback;
}

void ActorAnimationInstance::advance(float seconds)
{
	float time = m_Time;
	time += std::fmod(seconds, m_Range);
	if(time < m_Min)
	{
		if(m_Loop)
		{
			m_Animation->triggerEvents(m_Artboard, time, m_Time, m_Events);
			time = m_Max - (m_Min - time);
			m_Animation->triggerEvents(m_Artboard, time, m_Max, m_Events);
		}
		else
		{
			time = m_Min;
			if(m_Time != time)
			{
				m_Animation->triggerEvents(m_Artboard, m_Min, m_Time, m_Events);
			}
		}
	}
	else if(time > m_Max)
	{
		if(m_Loop)
		{
			m_Animation->triggerEvents(m_Artboard, time, m_Time, m_Events);
			time = m_Min + (time - m_Max);
			m_Animation->triggerEvents(m_Artboard, m_Min-0.001f, time, m_Events);
		}
		else
		{
			time = m_Max;
			if(m_Time != time)
			{
				m_Animation->triggerEvents(m_Artboard, m_Time, m_Max, m_Events);
			}
		}
	}
	else if(time > m_Time)
	{
		m_Animation->triggerEvents(m_Artboard, m_Time, time, m_Events);
	}
	else
	{
		m_Animation->triggerEvents(m_Artboard, time, m_Time, m_Events);
	}

	for(const ActorAnimationEvent& ev : m_Events)
	{
		if(m_EventCallback != nullptr)
		{
			m_EventCallback(ev, m_EventCallbackUserData);
		}
		if(m_Artboard->m_EventCallback != nullptr)
		{
			m_Artboard->m_EventCallback(ev, m_Artboard->m_EventCallbackUserData);
		}
        /*if (AnimationEvent != null)
        {
            AnimationEvent(this, ev);
        }
        m_Artboard.OnAnimationEvent(ev);*/
	}
	m_Events.clear();
	/*for(var i = 0; i < triggeredEvents.length; i++)
	{
		var event = triggeredEvents[i];
		this.dispatch("animationEvent", event);
		m_Artboard.dispatch("animationEvent", event);
	}*/
	m_Time = time;
}

void ActorAnimationInstance::apply(float mix)
{
	m_Animation->apply(m_Time, m_Artboard, mix);
}