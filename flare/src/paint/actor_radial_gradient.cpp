#include "flare/actor.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"
#include "flare/paint/actor_radial_gradient.hpp"

using namespace flare;

ActorRadialGradient::ActorRadialGradient(ComponentType type) : Base(type) {}

void ActorRadialGradient::copy(const ActorRadialGradient* gradient, ActorArtboard* artboard)
{
	Base::copy(gradient, artboard);
	m_SecondaryRadiusScale = gradient->m_SecondaryRadiusScale;
}

ActorRadialGradient* ActorRadialGradient::read(ActorArtboard* artboard, BlockReader* reader,
                                               ActorRadialGradient* component)
{
	Base::read(artboard, reader, component);
	component->m_SecondaryRadiusScale = reader->readFloat32();
	return component;
}

float ActorRadialGradient::secondaryRadiusScale() const
{
    return m_SecondaryRadiusScale;
}
void ActorRadialGradient::secondaryRadiusScale(float value)
{
    if(m_SecondaryRadiusScale == value)
    {
        return;
    }
    m_SecondaryRadiusScale = value;

    // TODO: support secondary radius
    // determine if we actually want to mark the paint dirty here to update any world values
    invalidatePaint();
}