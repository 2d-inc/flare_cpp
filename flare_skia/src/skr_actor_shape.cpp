#include "flare_skia/skr_actor_shape.hpp"
#include "flare/paint/actor_fill.hpp"
#include "flare/paint/actor_stroke.hpp"
#include "flare/path/actor_base_path.hpp"
#include "flare_skia/skr_actor_base_path.hpp"
#include "flare_skia/to_skia.hpp"
#include "flare_skia/skr_stroke.hpp"
#include "flare_skia/skr_fill.hpp"

using namespace flare;

SkrActorShape::SkrActorShape() : m_IsValid(false), m_Fill(nullptr), m_Stroke(nullptr) {}

void SkrActorShape::onBlendModeChanged(BlendMode from, BlendMode to)
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void SkrActorShape::onRenderOpacityChanged()
{
	// We mark the paints dirty here so that their update can change the blend modes.
	for (auto stroke : m_Strokes)
	{
		stroke->markPaintDirty();
	}
	for (auto fill : m_Fills)
	{
		fill->markPaintDirty();
	}
}

void SkrActorShape::onChildrenChanged()
{
	m_SubPaths.clear();
	auto kids = children();
	for (auto child : kids)
	{
		SkrActorBasePath* path = dynamic_cast<SkrActorBasePath*>(child);
		if (path == nullptr)
		{
			continue;
		}
		m_SubPaths.push_back(path);
	}
}

void SkrActorShape::invalidateDrawable() { m_IsValid = false; }

SkPath& SkrActorShape::path()
{
    if (!m_IsValid)
	{
		m_IsValid = true;
		m_Path.rewind();

		for (auto path : m_SubPaths)
		{
			const Mat2D& pathTransform = path->basePath()->pathTransform();
			m_Path.addPath(path->path(), ToSkia::convert(pathTransform));
		}
	}
    return m_Path;
}

void SkrActorShape::draw(SkCanvas* canvas)
{
	if (!doesDraw())
	{
		return;
	}

    SkPath& renderPath = path();

	canvas->save();

    auto clips = clippingShapes();
    for(auto& clipLayer : clips)
    {
        if(clipLayer.size() == 1)
        {
            canvas->clipPath(clipLayer[0]->path(), true);
        }
        else
        {
            SkPath complexClip;
            for(auto& clipShape : clipLayer)
            {
                complexClip.addPath(clipShape->path());
            }
            canvas->clipPath(complexClip, true);
        }
    }

    if(m_Fill != nullptr)
    {
        m_Fill->paint(canvas, renderPath);
    }
    if(m_Stroke != nullptr)
    {
        m_Stroke->paint(canvas, renderPath);
    }

	canvas->restore();
}

void SkrActorShape::onStrokesChanged()
{
    if(m_Strokes.size() == 0)
    {
        m_Stroke = nullptr;
    }
    else
    {
        m_Stroke = dynamic_cast<SkrStroke*>(m_Strokes[0]);
    }
}

void SkrActorShape::onFillsChanged()
{
    if(m_Fills.size() == 0)
    {
        m_Fill = nullptr;
    }
    else
    {
        m_Fill = dynamic_cast<SkrFill*>(m_Fills[0]);
    }
}
