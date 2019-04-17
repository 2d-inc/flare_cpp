#include "flare_skia/skr_color_fill.hpp"
#include "flare/actor_shape.hpp"
#include "flare_skia/to_skia.hpp"

using namespace flare;

void SkrColorFill::updatePaint()
{
    m_Paint.setBlendMode(ToSkia::convert(m_Shape->blendMode()));
    SkColor4f color{m_Color[0], m_Color[1], m_Color[2], m_Color[3] * m_RenderOpacity};
    m_Paint.setColor4f(color, nullptr);
    onPaintUpdated();
}