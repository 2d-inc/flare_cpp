#ifndef _SKR_RADIAL_GRADIENT_FILL_HPP_
#define _SKR_RADIAL_GRADIENT_FILL_HPP_

#include "flare/paint/actor_radial_gradient_fill.hpp"
#include "skr_fill.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class SkrRadialGradientFill : public ActorRadialGradientFill, public SkrFill
	{
		typedef ActorRadialGradientFill Base;

	public:
		void initializeGraphics() override { SkrFill::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif