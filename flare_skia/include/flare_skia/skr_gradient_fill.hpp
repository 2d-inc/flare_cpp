#ifndef _SKR_GRADIENT_FILL_HPP_
#define _SKR_GRADIENT_FILL_HPP_

#include "flare/paint/actor_gradient_fill.hpp"
#include "skr_fill.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class SkrGradientFill : public ActorGradientFill, public SkrFill
	{
        typedef ActorGradientFill Base;
        
	public:
		void initializeGraphics() override { SkrFill::initializeGraphics(); }
		void updatePaint() override;
	};
} // namespace flare

#endif