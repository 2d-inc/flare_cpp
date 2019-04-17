#ifndef _FLARE_ACTOR_GRADIENT_COLOR_HPP_
#define _FLARE_ACTOR_GRADIENT_COLOR_HPP_

#include "flare/paint/actor_paint.hpp"
#include "flare_math/vec2d.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;
	class KeyFrameGradient;
	class KeyFrameRadialGradient;

	class ActorGradientColor : public ActorPaint
	{
		friend class KeyFrameGradient;
		friend class KeyFrameRadialGradient;

		typedef ActorPaint Base;

	private:
		std::vector<float> m_ColorStops;
		Vec2D m_Start;
		Vec2D m_End;
		Vec2D m_RenderStart;
		Vec2D m_RenderEnd;

	public:
		const Vec2D& start() { return m_Start; }
		const Vec2D& end() { return m_End; }
		const Vec2D& renderStart() { return m_RenderStart; }
		const Vec2D& renderEnd() { return m_RenderEnd; }
		const std::vector<float>&  colorStops() { return m_ColorStops; }

		bool start(const Vec2D& value);
		bool end(const Vec2D& value);
		bool colorStops(const std::vector<float>& value);

		ActorGradientColor(ComponentType type);
		void copy(const ActorGradientColor* gradientColor, ActorArtboard* artboard);
		static ActorGradientColor* read(ActorArtboard* artboard, BlockReader* reader, ActorGradientColor* component);
		void update(DirtyFlags dirt) override;
	};
} // namespace flare

#endif