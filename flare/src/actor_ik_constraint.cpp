#include "flare/actor_ik_constraint.hpp"
#include "flare/actor.hpp"
#include <cmath>

using namespace flare;

ActorIKConstraint::ActorIKConstraint() : Base(nullptr, ComponentType::ActorIKConstraint) {}

ActorIKConstraint::ActorIKConstraint(ActorIKConstraint* target) : Base(nullptr, ComponentType::ActorIKConstraint) {}

ActorIKConstraint::InfluencedBone::InfluencedBone() : boneIndex(0) {}

ActorComponent* ActorIKConstraint::makeInstance(ActorArtboard* artboard) const
{
	ActorIKConstraint* instanceNode = new ActorIKConstraint();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorIKConstraint::copy(const ActorIKConstraint* constraint, ActorArtboard* artboard)
{
	Base::copy(constraint, artboard);

	m_InvertDirection = constraint->m_InvertDirection;
	m_InfluencedBones.resize(constraint->m_InfluencedBones.size());
	for (int i = 0, length = m_InfluencedBones.size(); i < length; i++)
	{
		InfluencedBone& ib = m_InfluencedBones[i];
		ib.boneIndex = constraint->m_InfluencedBones[i].boneIndex;
	}
}

void ActorIKConstraint::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	if (m_Parent != nullptr)
	{
		// This works because nodes are exported in hierarchy order, so we are assured constraints
		// get added in order as we resolve indices.
		m_Parent->addConstraint(this);
	}

	for (InfluencedBone& influenced : m_InfluencedBones)
	{
		influenced.bone = static_cast<ActorBone*>(components[influenced.boneIndex]);
	}
}

void ActorIKConstraint::completeResolve()
{
	Base::completeResolve();
	if (m_InfluencedBones.size() == 0)
	{
		return;
	}

	// Initialize solver.
	ActorBone* start = m_InfluencedBones[0].bone;
	ActorBone* end = m_InfluencedBones[m_InfluencedBones.size() - 1].bone;
	int count = 0;
	while (end != nullptr && end != start->parent())
	{
		count++;
		end = dynamic_cast<ActorBone*>(end->parent());
	}

	bool allIn = count < 3;
	end = m_InfluencedBones[m_InfluencedBones.size() - 1].bone;
	m_FKChain.resize(count);
	// Make sure bone data is cleared as it holds pointers to entries in the fk chain.
	m_BoneData.clear();

	int idx = count - 1;
	while (end != nullptr && end != start->parent())
	{
		BoneChain& bc = m_FKChain[idx--];
		bc.bone = end;
		bc.angle = 0.0f;
		bc.included = allIn;
		bc.index = idx + 1;
		end = dynamic_cast<ActorBone*>(end->parent());
	}

	// Make sure bones are good.
	for (InfluencedBone& influenced : m_InfluencedBones)
	{
		auto result =
		    std::find_if(m_FKChain.begin(), m_FKChain.end(), [influenced](const BoneChain& chainItem) -> bool {
			    return chainItem.bone == influenced.bone;
		    });
		if (result == m_FKChain.end())
		{
			printf("Bone not in chain %s\n", influenced.bone->name().c_str());
			continue;
		}
		// Note this indirection here, bone data and fk chain are linked and must be treated
		// accordingly.
		m_BoneData.push_back(&(*result));
	}

	if (!allIn)
	{
		// Influenced bones in bone data are in the IK chain.
		for (BoneChain* bone : m_BoneData)
		{
			bone->included = true;
		}
	}

	// Finally mark dependencies.
	for (InfluencedBone& influenced : m_InfluencedBones)
	{
		// Don't mark dependency on parent as ActorComponent already does this.
		if (influenced.bone == m_Parent)
		{
			continue;
		}

		m_Artboard->addDependency(this, influenced.bone);
	}

	if (m_Target != nullptr)
	{
		m_Artboard->addDependency(this, m_Target);
	}

	// All the first level children of the influenced bones should depend on the final bone.
	BoneChain& tip = *(m_FKChain.rbegin());
	// BoneChain tip = m_FKChain[m_FKChain.Length-1];
	for (BoneChain& fk : m_FKChain)
	{
		if (&fk == &tip)
		{
			continue;
		}

		ActorBone* bone = fk.bone;
		for (ActorNode* boneChild : bone->children())
		{
			auto result =
			    std::find_if(m_FKChain.begin(), m_FKChain.end(),
			                 [boneChild](const BoneChain& chainItem) -> bool { return chainItem.bone == boneChild; });

			if (result != m_FKChain.end())
			{
				// node is in the FK chain and is already dependent.
				continue;
			}

			m_Artboard->addDependency(boneChild, tip.bone);
		}
	}
}

ActorIKConstraint* ActorIKConstraint::read(ActorArtboard* artboard, BlockReader* reader, ActorIKConstraint* ik)
{
	if (ik == nullptr)
	{
		ik = new ActorIKConstraint();
	}

	Base::read(artboard, reader, ik);

	ik->m_InvertDirection = reader->readUint8() == 1;

	int numInfluencedBones = (int)reader->readUint8();
	if (numInfluencedBones > 0)
	{
		ik->m_InfluencedBones.resize(numInfluencedBones);
		for (InfluencedBone& influenced : ik->m_InfluencedBones)
		{
			influenced.boneIndex = reader->readUint16();
		}
	}
	return ik;
}

void ActorIKConstraint::solve1(BoneChain* fk1, const Vec2D& worldTargetTranslation)
{
	Mat2D iworld = fk1->parentWorldInverse;
	Vec2D pA;
	fk1->bone->worldTranslation(pA);
	Vec2D pBT(worldTargetTranslation);

	// To target in worldspace
	Vec2D toTarget;
	Vec2D::subtract(toTarget, pBT, pA);

	// Note this is directional, hence not transformMat2d
	Vec2D toTargetLocal;
	Vec2D::transformDir(toTargetLocal, toTarget, iworld);
	float r = std::atan2(toTargetLocal[1], toTargetLocal[0]);

	constrainRotation(fk1, r);
	fk1->angle = r;
}

void ActorIKConstraint::solve2(BoneChain* fk1, BoneChain* fk2, const Vec2D& worldTargetTranslation)
{
	ActorBone* b1 = fk1->bone;
	ActorBone* b2 = fk2->bone;
	BoneChain* firstChild = &(m_FKChain[fk1->index + 1]);

	const Mat2D& iworld = fk1->parentWorldInverse;

	Vec2D pA, pC, pB;
	b1->worldTranslation(pA);
	firstChild->bone->worldTranslation(pC);
	b2->tipWorldTranslation(pB);
	Vec2D pBT(worldTargetTranslation);

	Vec2D::transform(pA, pA, iworld);
	Vec2D::transform(pC, pC, iworld);
	Vec2D::transform(pB, pB, iworld);
	Vec2D::transform(pBT, pBT, iworld);

	// http://mathworld.wolfram.com/LawofCosines.html
	Vec2D av, bv, cv;
	Vec2D::subtract(av, pB, pC);
	float a = Vec2D::length(av);

	Vec2D::subtract(bv, pC, pA);
	float b = Vec2D::length(bv);

	Vec2D::subtract(cv, pBT, pA);
	float c = Vec2D::length(cv);

	float A = std::acos(std::max(-1.0f, std::min(1.0f, (-a * a + b * b + c * c) / (2.0f * b * c))));
	float C = std::acos(std::max(-1.0f, std::min(1.0f, (a * a + b * b - c * c) / (2.0f * a * b))));

	float r1, r2;
	if (b2->parent() != b1)
	{
		BoneChain& secondChild = m_FKChain[fk1->index + 2];

		const Mat2D& secondChildWorldInverse = secondChild.parentWorldInverse;

		firstChild->bone->worldTranslation(pC);
		b2->tipWorldTranslation(pB);

		Vec2D avec;
		Vec2D::subtract(avec, pB, pC);
		Vec2D avLocal;
		Vec2D::transformDir(avLocal, avec, secondChildWorldInverse);
		float angleCorrection = -std::atan2(avLocal[1], avLocal[0]);

		if (m_InvertDirection)
		{
			r1 = std::atan2(cv[1], cv[0]) - A;
			r2 = -C + M_PI + angleCorrection;
		}
		else
		{
			r1 = A + std::atan2(cv[1], cv[0]);
			r2 = C - M_PI + angleCorrection;
		}
	}
	else if (m_InvertDirection)
	{
		r1 = std::atan2(cv[1], cv[0]) - A;
		r2 = -C + M_PI;
	}
	else
	{
		r1 = A + std::atan2(cv[1], cv[0]);
		r2 = C - M_PI;
	}

	constrainRotation(fk1, r1);
	constrainRotation(firstChild, r2);
	if (firstChild != fk2)
	{
		ActorBone* bone = fk2->bone;
		Mat2D::multiply(bone->mutableWorldTransform(), bone->parent()->worldTransform(), bone->transform());
	}

	// Simple storage, need this for interpolation.
	fk1->angle = r1;
	firstChild->angle = r2;
}

void ActorIKConstraint::constrainRotation(BoneChain* fk, float rotation)
{
	ActorBone* bone = fk->bone;
	const Mat2D& parentWorld = bone->parent()->worldTransform();
	Mat2D& transform = bone->mutableTransform();
	TransformComponents& c = fk->transformComponents;

	if (rotation == 0.0f)
	{
		Mat2D::identity(transform);
	}
	else
	{
		Mat2D::fromRotation(transform, rotation);
	}
	// Translate
	transform[4] = c.x();
	transform[5] = c.y();
	// Scale
	float scaleX = c.scaleX();
	float scaleY = c.scaleY();
	transform[0] *= scaleX;
	transform[1] *= scaleX;
	transform[2] *= scaleY;
	transform[3] *= scaleY;

	// Skew
	const float skew = c.skew();
	if (skew != 0.0f)
	{
		transform[2] = transform[0] * skew + transform[2];
		transform[3] = transform[1] * skew + transform[3];
	}

	Mat2D::multiply(bone->mutableWorldTransform(), parentWorld, transform);
}

void ActorIKConstraint::constrain(ActorNode* node)
{
	// TODO: Should ActorTargetedConstraint just store targets as nodes?
	ActorNode* target = static_cast<ActorNode*>(m_Target);
	if (target == nullptr || m_InfluencedBones.empty())
	{
		return;
	}

	Vec2D worldTargetTranslation;
	target->worldTranslation(worldTargetTranslation);

	// Decompose the chain.
	for (BoneChain& item : m_FKChain)
	{
		ActorBone* bone = item.bone;
		const Mat2D& parentWorld = bone->parent()->worldTransform();
		Mat2D::invert(item.parentWorldInverse, parentWorld);

		Mat2D& boneTransform = bone->mutableTransform();
		Mat2D::multiply(boneTransform, item.parentWorldInverse, bone->worldTransform());
		Mat2D::decompose(item.transformComponents, boneTransform);
	}

	int count = m_BoneData.size();
	if (count == 1)
	{
		solve1(m_BoneData[0], worldTargetTranslation);
	}
	else if (count == 2)
	{
		solve2(m_BoneData[0], m_BoneData[1], worldTargetTranslation);
	}
	else
	{
		BoneChain* tip = m_BoneData[count - 1];
		for (int i = 0; i < count - 1; i++)
		{
			BoneChain* item = m_BoneData[i];
			solve2(item, tip, worldTargetTranslation);
			for (int j = item->index + 1, end = m_FKChain.size() - 1; j < end; j++)
			{
				BoneChain& fk = m_FKChain[j];
				Mat2D::invert(fk.parentWorldInverse, fk.bone->parent()->worldTransform());
			}
		}
	}

	// At the end, mix the FK angle with the IK angle by strength
	if (m_Strength != 1.0)
	{
		for (BoneChain& fk : m_FKChain)
		{
			if (!fk.included)
			{
				ActorBone* bone = fk.bone;
				Mat2D::multiply(bone->mutableWorldTransform(), bone->parent()->worldTransform(), bone->transform());
				continue;
			}
			float fromAngle = std::fmod(fk.transformComponents.rotation(), (float)M_PI_2);
			float toAngle = std::fmod(fk.angle, (float)M_PI_2);
			float diff = toAngle - fromAngle;
			if (diff > M_PI)
			{
				diff -= M_PI_2;
			}
			else if (diff < -M_PI)
			{
				diff += M_PI_2;
			}
			float angle = fromAngle + diff * m_Strength;
			constrainRotation(&fk, angle);
		}
	}
}