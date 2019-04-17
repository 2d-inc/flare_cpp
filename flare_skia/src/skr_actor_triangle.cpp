#include "flare_skia/skr_actor_triangle.hpp"

using namespace flare;

SkrActorTriangle::SkrActorTriangle() : SkrActorBasePath(this) {}

void SkrActorTriangle::invalidateDrawable() { m_IsPathValid = false; }