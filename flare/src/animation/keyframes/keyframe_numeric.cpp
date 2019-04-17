#include "flare/animation/keyframes/keyframe_numeric.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

KeyFrameNumeric::KeyFrameNumeric() : 
	m_Value(0.0f)
{

}

float KeyFrameNumeric::value() const
{
	return m_Value;
}

bool KeyFrameNumeric::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	m_Value = reader->readFloat32();

	return true;
}

void KeyFrameNumeric::apply(ActorComponent* component, float mix)
{
	this->setValue(component, m_Value, mix);
}

void KeyFrameNumeric::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	double f = (time - m_Time) / (toFrame->time() - m_Time);
	if(m_Interpolator != nullptr)
	{
		f = m_Interpolator->getEasedMix(f);
	}
	setValue(component, m_Value * (1.0 - f) + reinterpret_cast<KeyFrameNumeric*>(toFrame)->value() * f, mix);
}

KeyFrameInt::KeyFrameInt() : 
	m_Value(0)
{

}

int KeyFrameInt::value() const
{
	return m_Value;
}

bool KeyFrameInt::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	m_Value = reader->readInt32();

	return true;
}

void KeyFrameInt::apply(ActorComponent* component, float mix)
{
	this->setValue(component, m_Value, mix);
}

void KeyFrameInt::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	double f = (time - m_Time) / (toFrame->time() - m_Time);
	if(m_Interpolator != nullptr)
	{
		f = m_Interpolator->getEasedMix(f);
	}
	setValue(component, m_Value * (1.0 - f) + reinterpret_cast<KeyFrameNumeric*>(toFrame)->value() * f, mix);
}