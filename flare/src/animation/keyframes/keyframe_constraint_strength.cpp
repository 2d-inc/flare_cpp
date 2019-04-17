#include "flare/animation/keyframes/keyframe_constraint_strength.hpp"
#include "flare/actor_constraint.hpp"

using namespace flare;

void KeyFrameConstraintStrength::setValue(ActorComponent* component, float value, float mix)
{
	ActorConstraint* constraint = static_cast<ActorConstraint*>(component);
	constraint->strength(constraint->strength() * (1.0f - mix) + value * mix);
}