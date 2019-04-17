#include "flare_skia/skr_actor_star.hpp"

using namespace flare;

SkrActorStar::SkrActorStar() : SkrActorBasePath(this) {}

void SkrActorStar::invalidateDrawable() { m_IsPathValid = false; }