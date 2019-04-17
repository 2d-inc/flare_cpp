#ifndef _SKR_STROKE_HPP_
#define _SKR_STROKE_HPP_

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "flare/paint/actor_stroke.hpp"

namespace flare
{
	class SkrStroke
	{
	protected:
		SkPaint m_Paint;
        SkPath* m_EffectPath;
        ActorStroke* m_ActorStroke;

	public:
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(SkCanvas* canvas, SkPath& path);
	};
} // namespace flare
#endif