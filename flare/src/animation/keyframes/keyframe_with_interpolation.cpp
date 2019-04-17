#include "flare/animation/keyframes/keyframe_with_interpolation.hpp"
#include "flare/animation/interpolators/hold_interpolator.hpp"
#include "flare/animation/interpolators/linear_interpolator.hpp"
#include "flare/animation/interpolators/cubic_interpolator.hpp"
#include "flare/block_reader.hpp"
using namespace flare;

KeyFrameWithInterpolation::KeyFrameWithInterpolation() : m_Interpolator(nullptr) {}

KeyFrameWithInterpolation::~KeyFrameWithInterpolation()
{
	switch (interpolationType())
	{
		case InterpolationType::Cubic:
			delete m_Interpolator;
			break;
		default:
			break;
	}
}

InterpolationType KeyFrameWithInterpolation::interpolationType() const
{
	if (m_Interpolator == HoldInterpolator::instance())
	{
		return InterpolationType::Hold;
	}
	else if (m_Interpolator == LinearInterpolator::instance())
	{
		return InterpolationType::Linear;
	}
	else if(m_Interpolator != nullptr)
	{
		return InterpolationType::Cubic;
	}
	return InterpolationType::Max;
}

const KeyFrameInterpolator* KeyFrameWithInterpolation::interpolator() const { return m_Interpolator; }

bool KeyFrameWithInterpolation::read(BlockReader* reader, ActorComponent* component)
{
	if (!Base::read(reader, component))
	{
		return false;
	}
	int type = (int)reader->readUint8();

	if (type >= (int)InterpolationType::Max)
	{
		return false;
	}

	InterpolationType interpolationType = static_cast<InterpolationType>(type);
	switch (interpolationType)
	{
		case InterpolationType::Hold:
			m_Interpolator = HoldInterpolator::instance();
			break;
		case InterpolationType::Cubic:
		{
			CubicInterpolator* interpolator = new CubicInterpolator();
			if(interpolator->read(reader))
			{
				m_Interpolator = interpolator;
			}
			else
			{
				delete interpolator;
				// Make sure we fall back to a linear interpolator if the cubic is invalid (or is linear).
				m_Interpolator = LinearInterpolator::instance();
			}
			break;
		}

		case InterpolationType::Linear:
		default:
			m_Interpolator = LinearInterpolator::instance();
			break;
	}
	return true;
}

void KeyFrameWithInterpolation::setNext(KeyFrame* frame) {}