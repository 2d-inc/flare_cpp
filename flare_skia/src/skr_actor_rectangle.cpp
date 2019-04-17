#include "flare_skia/skr_actor_rectangle.hpp"

using namespace flare;

SkrActorRectangle::SkrActorRectangle() : SkrActorBasePath(this) {}

void SkrActorRectangle::invalidateDrawable() { m_IsPathValid = false; }