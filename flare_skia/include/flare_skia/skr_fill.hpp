#ifndef _SKR_FILL_HPP_
#define _SKR_FILL_HPP_

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "flare/paint/actor_fill.hpp"

namespace flare
{
	class SkrFill
	{
	protected:
		SkPaint m_Paint;
		ActorFill* m_ActorFill;

	public:
		void initializeGraphics();
		virtual void onPaintUpdated() {}
		void updatePaint();
		void paint(SkCanvas* canvas, SkPath& path);
	};
} // namespace flare
#endif