#include <math.h>
#include <iostream>
#include "flare/animation/keyframes/keyframe_sequence.hpp"
#include "flare/actor_image.hpp"

using namespace flare;

bool KeyFrameSequence::read(BlockReader* reader, ActorComponent* component)
{
    if(KeyFrameNumeric::read(reader, component))
    {
        return true;
    }

    return false;
}

void KeyFrameSequence::setValue(ActorComponent* component, float value, float mix)
{
    ActorImage* node = static_cast<ActorImage*>(component);
    int v = floor(value);
    int c = node->sequenceFramesCount();
    int frameIdx = v % c;
    if(frameIdx < 0)
    {
        frameIdx += node->sequenceFramesCount();
    }
    node->sequenceFrame(frameIdx);
}