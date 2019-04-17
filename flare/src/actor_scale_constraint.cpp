#include "flare/actor_scale_constraint.hpp"

using namespace flare;

ActorScaleConstraint::ActorScaleConstraint() : Base(nullptr, ComponentType::ActorScaleConstraint) {}

ActorComponent* ActorScaleConstraint::makeInstance(ActorArtboard* artboard) const
{
	ActorScaleConstraint* instanceNode = new ActorScaleConstraint();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

ActorScaleConstraint* ActorScaleConstraint::read(ActorArtboard* artboard, BlockReader* reader, ActorScaleConstraint* constraint)
{
	if (constraint == nullptr)
	{
		constraint = new ActorScaleConstraint();
	}

	Base::read(artboard, reader, constraint);

	return constraint;
}

void ActorScaleConstraint::constrain(ActorNode* node)
{
	// TODO: Should ActorTargetedConstraint just store targets as nodes?
	ActorNode* target = static_cast<ActorNode*>(m_Target);
	ActorNode* grandParent = m_Parent->parent();

	const Mat2D& transformA = m_Parent->worldTransform();
	Mat2D transformB;
	Mat2D::decompose(m_ComponentsA, transformA);
	if (target == nullptr)
	{
		Mat2D::copy(transformB, transformA);
		TransformComponents::copy(m_ComponentsB, m_ComponentsA);
	}
	else
	{
		Mat2D::copy(transformB, target->worldTransform());
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
		Mat2D::decompose(m_ComponentsB, transformB);

		if (!m_CopyX)
		{
			m_ComponentsB.scaleX(m_DestSpace == TransformSpace::Local ? 1.0f : m_ComponentsA.scaleX());
		}
		else
		{
			m_ComponentsB.scaleX(m_ComponentsB.scaleX() * m_ScaleX);
			if (m_Offset)
			{
				m_ComponentsB.scaleX(m_ComponentsB.scaleX() * m_Parent->scaleX());
			}
		}

		if (!m_CopyY)
		{
			m_ComponentsB.scaleY(m_DestSpace == TransformSpace::Local ? 0.0f : m_ComponentsA.scaleY());
		}
		else
		{
			m_ComponentsB.scaleY(m_ComponentsB.scaleY() * m_ScaleY);
			if (m_Offset)
			{
				m_ComponentsB.scaleY(m_ComponentsB.scaleY() * m_Parent->scaleY());
			}
		}

		if (m_DestSpace == TransformSpace::Local)
		{
			// Destination space is in parent transform coordinates.
			// Recompose the parent local transform and get it in world, then decompose the world
			// for interpolation.
			if (grandParent != nullptr)
			{
				Mat2D::compose(transformB, m_ComponentsB);
				Mat2D::multiply(transformB, grandParent->worldTransform(), transformB);
				Mat2D::decompose(m_ComponentsB, transformB);
			}
		}
	}

	bool clampLocal = m_MinMaxSpace == TransformSpace::Local && grandParent != nullptr;
	if (clampLocal)
	{
		// Apply min max in local space, so transform to local coordinates first.
		Mat2D::compose(transformB, m_ComponentsB);
		Mat2D inverse;
		if (!Mat2D::invert(inverse, grandParent->worldTransform()))
		{
			return;
		}
		Mat2D::multiply(transformB, inverse, transformB);
		Mat2D::decompose(m_ComponentsB, transformB);
	}
	if (m_EnableMaxX && m_ComponentsB.scaleX() > m_MaxX)
	{
		m_ComponentsB.scaleX(m_MaxX);
	}
	if (m_EnableMinX && m_ComponentsB.scaleX() < m_MinX)
	{
		m_ComponentsB.scaleX(m_MinX);
	}
	if (m_EnableMaxY && m_ComponentsB.scaleY() > m_MaxY)
	{
		m_ComponentsB.scaleY(m_MaxY);
	}
	if (m_EnableMinY && m_ComponentsB.scaleY() < m_MinY)
	{
		m_ComponentsB.scaleY(m_MinY);
	}
	if (clampLocal)
	{
		// Transform back to world.
		Mat2D::compose(transformB, m_ComponentsB);
		Mat2D::multiply(transformB, grandParent->worldTransform(), transformB);
		Mat2D::decompose(m_ComponentsB, transformB);
	}

	float ti = 1.0f - m_Strength;

	m_ComponentsB.rotation(m_ComponentsA.rotation());
	m_ComponentsB.x(m_ComponentsA.x());
	m_ComponentsB.y(m_ComponentsA.y());
	m_ComponentsB.scaleX(m_ComponentsA.scaleX() * ti + m_ComponentsB.scaleX() * m_Strength);
	m_ComponentsB.scaleY(m_ComponentsA.scaleY() * ti + m_ComponentsB.scaleY() * m_Strength);
	m_ComponentsB.skew(m_ComponentsA.skew());

	Mat2D::compose(m_Parent->mutableWorldTransform(), m_ComponentsB);
}
