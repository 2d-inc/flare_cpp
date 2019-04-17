#include "flare_skia/skr_actor_artboard.hpp"
#include "flare_skia/skr_actor.hpp"
#include "flare_skia/skr_drawable.hpp"
#include "flare_skia/skr_drawable_proxy.hpp"

using namespace flare;

SkrActorArtboard::SkrActorArtboard(SkrActor* actor) : ActorArtboard(actor) {}

void SkrActorArtboard::initializeGraphics()
{
	for (int i = 0; i < m_DrawableCount; i++)
	{
		m_Drawables[i]->drawable()->initializeGraphics();
	}
}

void SkrActorArtboard::draw(SkCanvas* canvas)
{
	for (int i = 0; i < m_DrawableCount; i++)
	{
		reinterpret_cast<SkrDrawableProxy*>(m_Drawables[i])->skrDrawable()->draw(canvas);
	}
}