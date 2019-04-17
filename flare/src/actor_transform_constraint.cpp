#include "flare/actor_transform_constraint.hpp"
#include "flare/actor_node.hpp"
#include "flare/block_reader.hpp"
#include <cmath>

using namespace flare;

ActorTransformConstraint::ActorTransformConstraint() : Base(nullptr, ComponentType::ActorTransformConstraint) {}

ActorComponent* ActorTransformConstraint::makeInstance(ActorArtboard* artboard) const
{
	ActorTransformConstraint* instanceNode = new ActorTransformConstraint();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorTransformConstraint::copy(const ActorTransformConstraint* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	m_SourceSpace = node->m_SourceSpace;
	m_DestSpace = node->m_DestSpace;
}

void ActorTransformConstraint::constrain(ActorNode* node)
{
	// TODO: Should ActorTargetedConstraint just store targets as nodes?
	ActorNode* target = static_cast<ActorNode*>(m_Target);
	if (target == nullptr)
	{
		return;
	}

	const Mat2D& transformA = m_Parent->worldTransform();
	Mat2D transformB(target->worldTransform());
	if (m_SourceSpace == TransformSpace::Local)
	{
		ActorNode* grandParent = target->parent();
		if (grandParent != nullptr)
		{
			Mat2D inverse;
			if (!Mat2D::invert(inverse, grandParent->worldTransform()))
			{
				return;
			}
			Mat2D::multiply(transformB, inverse, transformB);
		}
	}
	if (m_DestSpace == TransformSpace::Local)
	{
		ActorNode* grandParent = m_Parent->parent();
		if (grandParent != nullptr)
		{
			Mat2D::multiply(transformB, grandParent->worldTransform(), transformB);
		}
	}
	Mat2D::decompose(m_ComponentsA, transformA);
	Mat2D::decompose(m_ComponentsB, transformB);

	float angleA = std::fmod(m_ComponentsA.rotation(), (float)M_PI_2);
	float angleB = std::fmod(m_ComponentsB.rotation(), (float)M_PI_2);
	float diff = angleB - angleA;
	if (diff > M_PI)
	{
		diff -= M_PI_2;
	}
	else if (diff < -M_PI)
	{
		diff += M_PI_2;
	}

	float ti = 1.0f - m_Strength;

	m_ComponentsB.rotation(angleA + diff * m_Strength);
	m_ComponentsB.x(m_ComponentsA.x() * ti + m_ComponentsB.x() * m_Strength);
	m_ComponentsB.y(m_ComponentsA.y() * ti + m_ComponentsB.y() * m_Strength);
	m_ComponentsB.scaleX(m_ComponentsA.scaleX() * ti + m_ComponentsB.scaleX() * m_Strength);
	m_ComponentsB.scaleY(m_ComponentsA.scaleY() * ti + m_ComponentsB.scaleY() * m_Strength);
	m_ComponentsB.skew(m_ComponentsA.skew() * ti + m_ComponentsB.skew() * m_Strength);

	Mat2D::compose(m_Parent->mutableWorldTransform(), m_ComponentsB);
}

ActorTransformConstraint* ActorTransformConstraint::read(ActorArtboard* artboard, BlockReader* reader,
                                                         ActorTransformConstraint* constraint)
{
	if (constraint == nullptr)
	{
		constraint = new ActorTransformConstraint();
	}
	ActorTargetedConstraint::read(artboard, reader, constraint);

	constraint->m_SourceSpace = (TransformSpace)reader->readUint8();
	constraint->m_DestSpace = (TransformSpace)reader->readUint8();

	return constraint;
}