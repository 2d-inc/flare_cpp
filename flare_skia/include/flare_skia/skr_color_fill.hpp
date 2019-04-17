#ifndef _SKR_COLOR_FILL_HPP_
#define _SKR_COLOR_FILL_HPP_

#include "flare/paint/actor_color_fill.hpp"
#include "skr_fill.hpp"

namespace flare
{
	class SkrColorFill : public ActorColorFill, public SkrFill
	{
		typedef ActorColorFill Base;

	public:
		void initializeGraphics() override { SkrFill::initializeGraphics(); }
		void updatePaint() override;
	};

} // namespace flare

#endif