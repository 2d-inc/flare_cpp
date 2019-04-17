#include "flare/actor_translation_constraint.hpp"

using namespace flare;

ActorTranslationConstraint::ActorTranslationConstraint() : Base(nullptr, ComponentType::ActorTranslationConstraint) {}

ActorComponent* ActorTranslationConstraint::makeInstance(ActorArtboard* artboard) const
{
	ActorTranslationConstraint* instanceNode = new ActorTranslationConstraint();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

ActorTranslationConstraint* ActorTranslationConstraint::read(ActorArtboard* artboard, BlockReader* reader,
                                                             ActorTranslationConstraint* constraint)
{
	if (constraint == nullptr)
	{
		constraint = new ActorTranslationConstraint();
	}

	Base::read(artboard, reader, constraint);

	return constraint;
}

void ActorTranslationConstraint::constrain(ActorNode* node)
{
	// TODO: Should ActorTargetedConstraint just store targets as nodes?
	ActorNode* target = static_cast<ActorNode*>(m_Target);
	ActorNode* grandParent = m_Parent->parent();

	Mat2D& transformA = m_Parent->mutableWorldTransform();
	Vec2D translationA(transformA[4], transformA[5]);
	Vec2D translationB;
	if (target == nullptr)
	{
		Vec2D::copy(translationB, translationA);
	}
	else
	{
		Mat2D transformB(target->worldTransform());
		if (m_SourceSpace == TransformSpace::Local)
		{
			ActorNode* sourceGrandParent = target->parent();
			if (sourceGrandParent != nullptr)
			{
				Mat2D inverse;
				if (!Mat2D::invert(inverse, sourceGrandParent->worldTransform()))
				{
					return;
				}
				Mat2D::multiply(transformB, inverse, transformB);
			}
		}
		translationB[0] = transformB[4];
		translationB[1] = transformB[5];

		if (!m_CopyX)
		{
			translationB[0] = m_DestSpace == TransformSpace::Local ? 0.0f : translationA[0];
		}
		else
		{
			translationB[0] *= m_ScaleX;
			if (m_Offset)
			{
				translationB[0] += m_Parent->x();
			}
		}

		if (!m_CopyY)
		{
			translationB[1] = m_DestSpace == TransformSpace::Local ? 0.0f : translationA[1];
		}
		else
		{
			translationB[1] *= m_ScaleY;

			if (m_Offset)
			{
				translationB[1] += m_Parent->y();
			}
		}

		if (m_DestSpace == TransformSpace::Local)
		{
			// Destination space is in parent transform coordinates.
			if (grandParent != nullptr)
			{
				Vec2D::transform(translationB, translationB, grandParent->worldTransform());
			}
		}
	}

	bool clampLocal = m_MinMaxSpace == TransformSpace::Local && grandParent != nullptr;
	if (clampLocal)
	{
		// Apply min max in local space, so transform to local coordinates first.
		Mat2D invert;
		if (!Mat2D::invert(invert, grandParent->worldTransform()))
		{
			return;
		}
		// Get our target world coordinates in parent local.
		Vec2D::transform(translationB, translationB, invert);
	}
	if (m_EnableMaxX && translationB[0] > m_MaxX)
	{
		translationB[0] = m_MaxX;
	}
	if (m_EnableMinX && translationB[0] < m_MinX)
	{
		translationB[0] = m_MinX;
	}
	if (m_EnableMaxY && translationB[1] > m_MaxY)
	{
		translationB[1] = m_MaxY;
	}
	if (m_EnableMinY && translationB[1] < m_MinY)
	{
		translationB[1] = m_MinY;
	}
	if (clampLocal)
	{
		// Transform back to world.
		Vec2D::transform(translationB, translationB, grandParent->worldTransform());
	}

	float ti = 1.0f - m_Strength;

	// Just interpolate world translation
	transformA[4] = translationA[0] * ti + translationB[0] * m_Strength;
	transformA[5] = translationA[1] * ti + translationB[1] * m_Strength;
}