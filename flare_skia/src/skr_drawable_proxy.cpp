#include "flare/actor_drawable.hpp"
#include "flare_skia/skr_drawable_proxy.hpp"
#include "flare_skia/skr_drawable.hpp"

using namespace flare;

SkrDrawableProxy::SkrDrawableProxy(ActorDrawable* drawable) : Base(drawable), m_SkrDrawable(dynamic_cast<SkrDrawable*>(drawable))
{

}