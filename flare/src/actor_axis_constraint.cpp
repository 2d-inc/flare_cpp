#include "flare/actor_axis_constraint.hpp"
#include "flare/actor.hpp"
#include <cmath>

using namespace flare;

ActorAxisConstraint::ActorAxisConstraint(ActorArtboard* artboard, ComponentType type) :
    Base(artboard, type),
    m_CopyX(0.0f),
    m_CopyY(0.0f),
    m_ScaleX(1.0f),
    m_ScaleY(1.0f),
    m_EnableMinX(false),
    m_EnableMinY(false),
    m_EnableMaxX(false),
    m_EnableMaxY(false),
    m_MaxX(0.0f),
    m_MaxY(0.0f),
    m_MinX(0.0f),
    m_MinY(0.0f),
    m_Offset(false),
    m_SourceSpace(TransformSpace::World),
    m_DestSpace(TransformSpace::World),
    m_MinMaxSpace(TransformSpace::World)
{
}

void ActorAxisConstraint::copy(const ActorAxisConstraint* constraint, ActorArtboard* artboard)
{
	Base::copy(constraint, artboard);

	m_CopyX = constraint->m_CopyX;
	m_CopyY = constraint->m_CopyY;
	m_ScaleX = constraint->m_ScaleX;
	m_ScaleY = constraint->m_ScaleY;
	m_EnableMinX = constraint->m_EnableMinX;
	m_EnableMinY = constraint->m_EnableMinY;
	m_EnableMaxX = constraint->m_EnableMaxX;
	m_EnableMaxY = constraint->m_EnableMaxY;
	m_MinX = constraint->m_MinX;
	m_MinY = constraint->m_MinY;
	m_MaxX = constraint->m_MaxX;
	m_MaxY = constraint->m_MaxY;
	m_Offset = constraint->m_Offset;
	m_SourceSpace = constraint->m_SourceSpace;
	m_DestSpace = constraint->m_DestSpace;
	m_MinMaxSpace = constraint->m_MinMaxSpace;
}

ActorAxisConstraint* ActorAxisConstraint::read(ActorArtboard* artboard, BlockReader* reader, ActorAxisConstraint* constraint)
{
	Base::read(artboard, reader, constraint);

	// X Axis
	if ((constraint->m_CopyX = reader->readUint8() == 1))
	{
		constraint->m_ScaleX = reader->readFloat32();
	}
	if ((constraint->m_EnableMinX = reader->readUint8() == 1))
	{
		constraint->m_MinX = reader->readFloat32();
	}
	if ((constraint->m_EnableMaxX = reader->readUint8() == 1))
	{
		constraint->m_MaxX = reader->readFloat32();
	}

	// Y Axis
	if ((constraint->m_CopyY = reader->readUint8() == 1))
	{
		constraint->m_ScaleY = reader->readFloat32();
	}
	if ((constraint->m_EnableMinY = reader->readUint8() == 1))
	{
		constraint->m_MinY = reader->readFloat32();
	}
	if ((constraint->m_EnableMaxY = reader->readUint8() == 1))
	{
		constraint->m_MaxY = reader->readFloat32();
	}

	constraint->m_Offset = reader->readUint8() == 1;
	constraint->m_SourceSpace = (TransformSpace)reader->readUint8();
	constraint->m_DestSpace = (TransformSpace)reader->readUint8();
	constraint->m_MinMaxSpace = (TransformSpace)reader->readUint8();

	return constraint;
}