#include "flare_skia/skr_actor_path.hpp"

using namespace flare;

SkrActorPath::SkrActorPath() : SkrActorBasePath(this) {}

void SkrActorPath::invalidateDrawable()
{
	Base::invalidateDrawable();
	m_IsPathValid = false;
}