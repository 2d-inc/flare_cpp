#include "flare/animation/interpolators/cubic_interpolator.hpp"

using namespace flare;

bool CubicInterpolator::read(BlockReader* reader)
{
    float x1 = reader->readFloat32();
    float y1 = reader->readFloat32();
    float x2 = reader->readFloat32();
    float y2 = reader->readFloat32();
    if(x1 == y1 && x2 == y2)
    {
        // we're linear
        return false;
    }
    initialize(x1, y1, x2, y2);
    return true;
}