#ifndef _SKR_COLOR_STROKE_HPP_
#define _SKR_COLOR_STROKE_HPP_

#include "flare/paint/actor_color_stroke.hpp"
#include "skr_stroke.hpp"

namespace flare
{
	class SkrColorStroke : public ActorColorStroke, public SkrStroke
	{
		typedef ActorColorStroke Base;

	public:
		void initializeGraphics() override { SkrStroke::initializeGraphics(); }
		void updatePaint() override;
	};

} // namespace flare

#endif