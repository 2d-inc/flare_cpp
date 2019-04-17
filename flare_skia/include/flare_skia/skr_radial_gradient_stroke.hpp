#ifndef _SKR_RADIAL_GRADIENT_STROKE_HPP_
#define _SKR_RADIAL_GRADIENT_STROKE_HPP_

#include "flare/paint/actor_radial_gradient_stroke.hpp"
#include "skr_stroke.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class SkrRadialGradientStroke : public ActorRadialGradientStroke, public SkrStroke
	{
        typedef ActorRadialGradientStroke Base;
        
	public:
		void initializeGraphics() override { SkrStroke::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif