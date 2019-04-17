#include "flare_skia/skr_actor_ellipse.hpp"

using namespace flare;

SkrActorEllipse::SkrActorEllipse() : SkrActorBasePath(this) {}

void SkrActorEllipse::invalidateDrawable() { m_IsPathValid = false; }