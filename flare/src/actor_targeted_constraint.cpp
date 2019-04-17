#include "flare/actor_targeted_constraint.hpp"
#include "flare/actor.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorTargetedConstraint::ActorTargetedConstraint(ActorArtboard* artboard, ComponentType type) :
    Base(artboard, type),
    m_TargetIdx(0)
{
}

void ActorTargetedConstraint::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	if (m_TargetIdx != 0)
	{
		m_Target = components[m_TargetIdx];
		if (m_Target != nullptr)
		{
			m_Artboard->addDependency(m_Parent, m_Target);
		}
	}
}

void ActorTargetedConstraint::copy(const ActorTargetedConstraint* constraint, ActorArtboard* artboard)
{
	Base::copy(constraint, artboard);

	m_TargetIdx = constraint->m_TargetIdx;
}

ActorTargetedConstraint* ActorTargetedConstraint::read(ActorArtboard* artboard, BlockReader* reader,
                                                       ActorTargetedConstraint* constraint)
{
	Base::read(artboard, reader, constraint);
	constraint->m_TargetIdx = reader->readUint16();

	return constraint;
}