#ifndef _FLARE_JELLYCOMPONENT_HPP_
#define _FLARE_JELLYCOMPONENT_HPP_

#include "actor_component.hpp"
#include "flare_math/vec2d.hpp"
#include <cmath>
#include <vector>

namespace flare
{
	class ActorArtboard;
	class BlockReader;
	class ActorJellyBone;

	class JellyComponent : public ActorComponent
	{
		typedef ActorComponent Base;

	public:
		static constexpr int JellyMax = 16;
		static const float OptimalDistance;
		static const float CurveConstant;

	private:
		float m_EaseIn;
		float m_EaseOut;
		float m_ScaleIn;
		float m_ScaleOut;
		unsigned short m_InTargetIdx;
		unsigned short m_OutTargetIdx;
		ActorNode* m_InTarget;
		ActorNode* m_OutTarget;
		std::vector<ActorJellyBone*> m_Bones;
		Vec2D m_InPoint;
		Vec2D m_InDirection;
		Vec2D m_OutPoint;
		Vec2D m_OutDirection;

		Vec2D m_CachedTip;
		Vec2D m_CachedOut;
		Vec2D m_CachedIn;
		float m_CachedScaleIn;
		float m_CachedScaleOut;

		Vec2D m_JellyPoints[JellyMax + 1];
		std::vector<Vec2D> m_NormalizedCurvePoints;

	public:
		JellyComponent();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const JellyComponent* node, ActorArtboard* artboard);
		static JellyComponent* read(ActorArtboard* artboard, BlockReader* reader, JellyComponent* node = nullptr);
		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		void completeResolve() override;
		void updateJellies();
		void update(DirtyFlags dirt) override;
	};
} // namespace flare
#endif