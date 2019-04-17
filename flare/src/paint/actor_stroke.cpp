#include "flare/actor_artboard.hpp"
#include "flare/block_reader.hpp"
#include "flare/paint/actor_stroke.hpp"

using namespace flare;

ActorStroke::ActorStroke() :
    m_Width(1.0f),
    m_Cap(StrokeCap::Butt),
    m_Join(StrokeJoin::Miter),
    m_TrimPath(TrimPath::Off),
    m_TrimStart(0.0f),
    m_TrimEnd(0.0f),
    m_TrimOffset(0.0f)
{
}

void ActorStroke::width(float value)
{
	if (m_Width == value)
	{
		return;
	}
	m_Width = value;
	markPaintDirty();
}

void ActorStroke::cap(StrokeCap value)
{
	if (m_Cap == value)
	{
		return;
	}
	m_Cap = value;
	markPaintDirty();
}

void ActorStroke::join(StrokeJoin value)
{
	if (m_Join == value)
	{
		return;
	}
	m_Join = value;
	markPaintDirty();
}

void ActorStroke::trimPath(TrimPath value)
{
	if (m_TrimPath == value)
	{
		return;
	}
	m_TrimPath = value;
	markPathEffectsDirty();
}

void ActorStroke::copy(const ActorStroke* stroke, ActorArtboard* artboard)
{
	m_Width = stroke->m_Width;
	m_Cap = stroke->m_Cap;
	m_Join = stroke->m_Join;
	m_TrimPath = stroke->m_TrimPath;
	m_TrimStart = stroke->m_TrimStart;
	m_TrimEnd = stroke->m_TrimEnd;
	m_TrimOffset = stroke->m_TrimOffset;
	markPaintDirty();
	markPathEffectsDirty();
}

ActorStroke* ActorStroke::read(ActorArtboard* artboard, BlockReader* reader, ActorStroke* stroke)
{
	stroke->m_Width = reader->readFloat32();
	stroke->m_Cap = (StrokeCap)reader->readUint8();
	stroke->m_Join = (StrokeJoin)reader->readUint8();
	stroke->m_TrimPath = (TrimPath)reader->readUint8();
	if (stroke->m_TrimPath != TrimPath::Off)
	{
		stroke->m_TrimStart = reader->readFloat32();
		stroke->m_TrimEnd = reader->readFloat32();
		stroke->m_TrimOffset = reader->readFloat32();
	}
	return stroke;
}

bool ActorStroke::trimStart(float value)
{
	if(value == m_TrimStart)
	{
		return false;
	}
	m_TrimStart = value;
	markPathEffectsDirty();
	return true;
}

bool ActorStroke::trimEnd(float value)
{
	if(value == m_TrimEnd)
	{
		return false;
	}
	m_TrimEnd = value;
	markPathEffectsDirty();
	return true;
}

bool ActorStroke::trimOffset(float value)
{
	if(value == m_TrimOffset)
	{
		return false;
	}
	m_TrimOffset = value;
	markPathEffectsDirty();
	return true;
}