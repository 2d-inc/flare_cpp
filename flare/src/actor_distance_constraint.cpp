#include "flare/actor_distance_constraint.hpp"
#include "flare/actor_node.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorDistanceConstraint::ActorDistanceConstraint() :
    Base(nullptr, ComponentType::ActorDistanceConstraint),
    m_Mode(Mode::Closer),
    m_Distance(100.0f)
{
}

void ActorDistanceConstraint::copy(const ActorDistanceConstraint* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	m_Mode = node->m_Mode;
	m_Distance = node->m_Distance;
}

ActorComponent* ActorDistanceConstraint::makeInstance(ActorArtboard* artboard) const
{
	ActorDistanceConstraint* instance = new ActorDistanceConstraint();
	instance->copy(this, artboard);
	return instance;
}

ActorDistanceConstraint* ActorDistanceConstraint::read(ActorArtboard* artboard, BlockReader* reader,
                                                       ActorDistanceConstraint* constraint)
{
	if (constraint == nullptr)
	{
		constraint = new ActorDistanceConstraint();
	}
	Base::read(artboard, reader, constraint);

	constraint->m_Distance = reader->readFloat32();
	constraint->m_Mode = (Mode)reader->readUint8();

	return constraint;
}

void ActorDistanceConstraint::constrain(ActorNode* node)
{
	// TODO: Should ActorTargetedConstraint just store targets as nodes?
	ActorNode* target = static_cast<ActorNode*>(m_Target);
	if (target == nullptr)
	{
		return;
	}

	Vec2D targetTranslation;
	target->worldTranslation(targetTranslation);

	Vec2D ourTranslation;
	m_Parent->worldTranslation(ourTranslation);

	Vec2D toTarget;
	Vec2D::subtract(toTarget, ourTranslation, targetTranslation);
	float currentDistance = Vec2D::length(toTarget);
	switch (m_Mode)
	{
		case Mode::Closer:
			if (currentDistance < m_Distance)
			{
				return;
			}
			break;
		case Mode::Further:
			if (currentDistance > m_Distance)
			{
				return;
			}
			break;
		default:
			break;
	}
	if (currentDistance < 0.001f)
	{
		return;
	}

	Vec2D::scale(toTarget, toTarget, 1.0f / currentDistance);
	Vec2D::scale(toTarget, toTarget, m_Distance);

	Mat2D& world = m_Parent->mutableWorldTransform();
	Vec2D position;
	Vec2D::add(position, targetTranslation, toTarget);
	Vec2D::lerp(position, ourTranslation, position, m_Strength);
	world[4] = position[0];
	world[5] = position[1];
}