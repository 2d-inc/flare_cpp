#ifndef _SKR_GRADIENT_STROKE_HPP_
#define _SKR_GRADIENT_STROKE_HPP_

#include "flare/paint/actor_gradient_stroke.hpp"
#include "skr_stroke.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class SkrGradientStroke : public ActorGradientStroke, public SkrStroke
	{
		typedef ActorGradientStroke Base;

	public:
		void initializeGraphics() override { SkrStroke::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif