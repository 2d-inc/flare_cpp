#include "flare_skia/skr_actor_polygon.hpp"

using namespace flare;

SkrActorPolygon::SkrActorPolygon() : SkrActorBasePath(this) {}

void SkrActorPolygon::invalidateDrawable() { m_IsPathValid = false; }