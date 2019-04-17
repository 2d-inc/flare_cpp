#include "flare/animation/keyframes/keyframe_color.hpp"
#include "flare/block_reader.hpp"
#include "flare/paint/actor_color.hpp"

using namespace flare;

KeyFrameColor::KeyFrameColor() : m_Value{0.0f} {}

bool KeyFrameColor::read(BlockReader* reader, ActorComponent* component)
{
	if (dynamic_cast<ActorColor*>(component) == nullptr || !Base::read(reader, component))
	{
		return false;
	}
	reader->readFloat32Array(m_Value, 4);
	return true;
}

void KeyFrameColor::apply(ActorComponent* component, float mix)
{
	ActorColor* actorColor = static_cast<ActorColor*>(component);
	if (actorColor == nullptr)
	{
		return;
	}
	Color wr;
	if (mix == 1.0)
	{
		for (int i = 0; i < 4; i++)
		{
			wr[i] = m_Value[i];
		}
	}
	else
	{
		float mixi = 1.0 - mix;
		for (int i = 0; i < 4; i++)
		{
			wr[i] = wr[i] * mixi + m_Value[i] * mix;
		}
	}
	actorColor->color(wr);
}

void KeyFrameColor::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	ActorColor* actorColor = static_cast<ActorColor*>(component);
	if (actorColor == nullptr)
	{
		return;
	}
	Color wr;
	float* to = reinterpret_cast<KeyFrameColor*>(toFrame)->m_Value;

	float f = (time - m_Time) / (toFrame->time() - m_Time);
	if (m_Interpolator != nullptr)
	{
		f = m_Interpolator->getEasedMix(f);
	}
	float fi = 1.0 - f;
	if (mix == 1.0)
	{
		for (int i = 0; i < 4; i++)
		{
			wr[i] = m_Value[i] * fi + to[i] * f;
		}
	}
	else
	{
		float mixi = 1.0 - mix;
		for (int i = 0; i < 4; i++)
		{
			float v = m_Value[i] * fi + to[i] * f;

			wr[i] = wr[i] * mixi + v * mix;
		}
	}
	actorColor->color(wr);
}