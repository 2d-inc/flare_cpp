#include "flare/animation/keyframes/keyframe.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

KeyFrame::KeyFrame() :
	m_Time(0.0f)
{

}

KeyFrame::~KeyFrame()
{

}

float KeyFrame::time() const
{
	return m_Time;
}

bool KeyFrame::read(BlockReader* reader, ActorComponent* component)
{
	m_Time = (float)reader->readFloat64();

	return true;
}

