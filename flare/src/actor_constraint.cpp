#include "flare/actor_constraint.hpp"
#include "flare/actor.hpp"
using namespace flare;

ActorConstraint::ActorConstraint(ActorArtboard* artboard, ComponentType type) :
    ActorComponent(artboard, type),
    m_IsEnabled(false),
    m_Strength(1.0f)
{
}

bool ActorConstraint::isEnabled() const { return m_IsEnabled; }

float ActorConstraint::strength() const { return m_Strength; }

void ActorConstraint::strength(float value)
{
	if (m_Strength == value)
	{
		return;
	}
	m_Strength = value;
	m_Parent->markTransformDirty();
}

void ActorConstraint::onDirty(DirtyFlags dirt) { m_Parent->markTransformDirty(); }

void ActorConstraint::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	if (m_Parent != nullptr)
	{
		// This works because nodes are exported in hierarchy order, so we are
		// assured constraints get added in order as we resolve indices.
		m_Parent->addConstraint(this);
	}
}

void ActorConstraint::copy(const ActorConstraint* constraint, ActorArtboard* artboard)
{
	Base::copy(constraint, artboard);

	m_IsEnabled = constraint->m_IsEnabled;
	m_Strength = constraint->m_Strength;
}

ActorConstraint* ActorConstraint::read(ActorArtboard* artboard, BlockReader* reader, ActorConstraint* constraint)
{
	Base::read(artboard, reader, constraint);
	constraint->m_Strength = reader->readFloat32();
	constraint->m_IsEnabled = reader->readUint8() == 1;

	return constraint;
}