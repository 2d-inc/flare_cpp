#include "flare/path/actor_procedural_path.hpp"
#include "flare/actor_shape.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorProceduralPath::ActorProceduralPath(ComponentType type) : Base(type) {}
ActorProceduralPath::~ActorProceduralPath() { invalidateDrawable(); }
void ActorProceduralPath::width(float value)
{
	if (m_Width == value)
	{
		return;
	}
	m_Width = value;
	invalidateDrawable();
}

void ActorProceduralPath::height(float value)
{
	if (m_Height == value)
	{
		return;
	}
	m_Height = value;
	invalidateDrawable();
}

void ActorProceduralPath::copy(const ActorProceduralPath* path, ActorArtboard* resetActor)
{
	Base::copy(path, resetActor);
	m_Width = path->m_Width;
	m_Height = path->m_Height;
}

void ActorProceduralPath::onDirty(DirtyFlags dirt)
{
	Base::onDirty(dirt);
	if (shape() != nullptr)
	{
		shape()->invalidateDrawable();
	}
}

void ActorProceduralPath::invalidateDrawable()
{
	if (shape() != nullptr)
	{
		shape()->invalidateDrawable();
	}
	for (auto point : m_Points)
	{
		delete point;
	}
	m_Points.clear();
}

const std::vector<PathPoint*>& ActorProceduralPath::deformedPoints() { return points(); }

const std::vector<PathPoint*>& ActorProceduralPath::points()
{
	if (m_Points.empty())
	{
		makePoints();
	}
	return m_Points;
}

ActorProceduralPath* ActorProceduralPath::read(ActorArtboard* artboard, BlockReader* reader, ActorProceduralPath* component)
{
	Base::read(artboard, reader, component);

	component->m_Width = reader->readFloat32();
	component->m_Height = reader->readFloat32();
	return component;
}