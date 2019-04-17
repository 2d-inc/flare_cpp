#include "flare_skia/skr_stroke.hpp"
#include "flare/paint/actor_stroke.hpp"
#include "flare_skia/to_skia.hpp"

using namespace flare;

void SkrStroke::initializeGraphics()
{
    m_ActorStroke = dynamic_cast<ActorStroke*>(this);
    m_Paint.setStyle(SkPaint::Style::kStroke_Style);
    m_Paint.setAntiAlias(true);
	assert(m_ActorStroke != nullptr);
}

void SkrStroke::updatePaint()
{
	m_Paint.setStrokeWidth(m_ActorStroke->width());
	m_Paint.setStrokeCap(ToSkia::convert(m_ActorStroke->cap()));
	m_Paint.setStrokeJoin(ToSkia::convert(m_ActorStroke->join()));
}

void SkrStroke::paint(SkCanvas* canvas, SkPath& path)
{
    if(m_ActorStroke->width() == 0)
    {
        return;
    }
    m_ActorStroke->validatePaint();
    if(m_ActorStroke->trimPath() != TrimPath::Off)
    {
        // TODO: Trim Path
    }
	canvas->drawPath(path, m_Paint);
}