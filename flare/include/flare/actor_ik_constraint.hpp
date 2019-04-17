#ifndef _FLARE_ACTORIKCONSTRAINT_HPP_
#define _FLARE_ACTORIKCONSTRAINT_HPP_

#include "actor_bone.hpp"
#include "actor_targeted_constraint.hpp"
#include "flare_math/transform_components.hpp"
#include "flare_math/mat2d.hpp"
#include <vector>

namespace flare
{
	class Actor;

	class ActorIKConstraint : public ActorTargetedConstraint
	{
		typedef ActorTargetedConstraint Base;

	public:
		union InfluencedBone
		{
			unsigned short boneIndex;
			ActorBone* bone;

			InfluencedBone();
		};

		struct BoneChain
		{
			int index;
			ActorBone* bone;
			float angle;
			bool included;
			TransformComponents transformComponents;
			Mat2D parentWorldInverse;
		};

	private:
		std::vector<InfluencedBone> m_InfluencedBones;
		std::vector<BoneChain> m_FKChain;
		std::vector<BoneChain*> m_BoneData;
		bool m_InvertDirection;

		void solve1(BoneChain* fk1, const Vec2D& worldTargetTranslation);
		void solve2(BoneChain* fk1, BoneChain* fk2, const Vec2D& worldTargetTranslation);
		void constrainRotation(BoneChain* fk, float rotation);

	public:
		ActorIKConstraint();
		ActorIKConstraint(ActorIKConstraint* target); // Support old system.

		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorIKConstraint* node, ActorArtboard* artboard);
		void resolveComponentIndices(ActorComponent** nodes, int numComponents) override;
		void completeResolve() override;
		void constrain(ActorNode* node) override;

		static ActorIKConstraint* read(ActorArtboard* artboard, BlockReader* reader, ActorIKConstraint* node = nullptr);
	};
} // namespace flare

#endif