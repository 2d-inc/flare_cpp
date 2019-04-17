#include "flare/path/actor_path.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_shape.hpp"
#include "flare/actor_skin.hpp"
#include "flare/block_reader.hpp"
#include "flare/path/path_point.hpp"

using namespace flare;

Mat2D identity;

ActorPath::ActorPath() : Base(ComponentType::ActorPath), m_VertexDeform(nullptr) {}
ActorPath::~ActorPath()
{
	for (auto point : m_Points)
	{
		delete point;
	}
	m_Points.clear();

	for (auto point : m_DeformedPoints)
	{
		delete point;
	}
	m_DeformedPoints.clear();
	delete[] m_VertexDeform;
}

const Mat2D& ActorPath::pathTransform() const { return isConnectedToBones() ? identity : worldTransform(); }

void ActorPath::copy(const ActorPath* path, ActorArtboard* artboard)
{
	Base::copy(path, artboard);
	copySkinnable(path, artboard);

	m_IsHidden = path->m_IsHidden;
	m_IsClosed = path->m_IsClosed;

	int pointCount = path->m_Points.size();
	m_Points.resize(pointCount);
	for (int i = 0; i < pointCount; i++)
	{
		m_Points[i] = path->m_Points[i]->makeInstance();
	}

	m_VertexDeformLength = path->m_VertexDeformLength;
	if (path->m_VertexDeform != nullptr)
	{
		m_VertexDeform = new float[path->m_VertexDeformLength];
		memcpy(m_VertexDeform, path->m_VertexDeform, m_VertexDeformLength * sizeof(float));
	}
}

void ActorPath::onDirty(DirtyFlags dirt)
{
	Base::onDirty(dirt);
	if (shape() != nullptr)
	{
		shape()->invalidateDrawable();
	}
}

void ActorPath::invalidateDrawable()
{
	if (shape() != nullptr)
	{
		shape()->invalidateDrawable();
	}
}

const std::vector<PathPoint*>& ActorPath::deformedPoints()
{
	if (!isConnectedToBones() || skin() == nullptr)
	{
		return points();
	}

	// Clear out old deform points.
	for (auto point : m_DeformedPoints)
	{
		delete point;
	}
	m_DeformedPoints.clear();

	auto world = worldTransform();
	const float* boneMatrices = skin()->boneMatrices();
	for (auto point : m_Points)
	{
		m_DeformedPoints.push_back(point->skin(world, boneMatrices));
	}

	return m_DeformedPoints;
}

void ActorPath::makeVertexDeform()
{
	if (m_VertexDeform != nullptr)
	{
		return;
	}
	m_VertexDeformLength = 0;
	for (auto point : m_Points)
	{
		m_VertexDeformLength += 2; // translation
		switch (point->type())
		{
			case PointType::Straight:
				m_VertexDeformLength += 1; // radius
				break;
			default:
				m_VertexDeformLength += 4; // in/out
				break;
		}
	}

	m_VertexDeform = new float[m_VertexDeformLength];

	int idx = 0;
	for (auto point : m_Points)
	{
		m_VertexDeform[idx++] = point->translation()[0];
		m_VertexDeform[idx++] = point->translation()[1];

		switch (point->type())
		{
			case PointType::Straight:
				m_VertexDeform[idx++] = reinterpret_cast<StraightPathPoint*>(point)->radius();
				break;
			default:
			{
				CubicPathPoint* cubicPoint = reinterpret_cast<CubicPathPoint*>(point);
				m_VertexDeform[idx++] = cubicPoint->inPoint()[0];
				m_VertexDeform[idx++] = cubicPoint->inPoint()[1];
				m_VertexDeform[idx++] = cubicPoint->outPoint()[0];
				m_VertexDeform[idx++] = cubicPoint->outPoint()[1];
				break;
			}
		}
	}
}

void ActorPath::markVertexDeformDirty()
{
	if (m_Artboard == nullptr)
	{
		return;
	}
	m_Artboard->addDirt(this, DirtyFlags::VertexDeformDirty, false);
	m_DeformedPoints.reserve(m_Points.size());
}

void ActorPath::update(DirtyFlags dirt)
{
	if (m_VertexDeform != nullptr && (dirt & DirtyFlags::VertexDeformDirty) == DirtyFlags::VertexDeformDirty)
	{
		int idx = 0;
		for (auto point : m_Points)
		{
			Vec2D& translation = point->translation();
			translation[0] = m_VertexDeform[idx++];
			translation[1] = m_VertexDeform[idx++];
			switch (point->type())
			{
				case PointType::Straight:
					reinterpret_cast<StraightPathPoint*>(point)->radius(m_VertexDeform[idx++]);
					break;

				default:
				{
					CubicPathPoint* cubicPoint = reinterpret_cast<CubicPathPoint*>(point);
					Vec2D& inPoint = cubicPoint->inPoint();
					Vec2D& outPoint = cubicPoint->outPoint();
					inPoint[0] = m_VertexDeform[idx++];
					inPoint[1] = m_VertexDeform[idx++];
					outPoint[0] = m_VertexDeform[idx++];
					outPoint[1] = m_VertexDeform[idx++];
					break;
				}
			}
		}
		// Invalidate the entire drawable (path & shape).
		invalidateDrawable();
	}

	Base::update(dirt);
}

ActorPath* ActorPath::read(ActorArtboard* artboard, BlockReader* reader, ActorPath* component)
{
	Base::read(artboard, reader, component);
	ActorSkinnable::read(artboard, reader, component);

	component->m_IsHidden = reader->readUint8() == 0;
	component->m_IsClosed = reader->readUint8() == 1;

	int pointCount = reader->readUint16();
	component->m_Points.resize(pointCount);
	for (int i = 0; i < pointCount; i++)
	{
		PathPoint* point = nullptr;
		PointType type = (PointType)reader->readUint8();
		switch (type)
		{
			case PointType::Straight:
				point = new StraightPathPoint();
				break;
			default:
				point = new CubicPathPoint(type);
				break;
		}

		point->read(reader, component->isConnectedToBones());
		component->m_Points[i] = point;
	}
	return component;
}

void ActorPath::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	resolveSkinnable(components, numComponents);
}