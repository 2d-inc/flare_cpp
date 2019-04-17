#include "flare/path/path_point.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

PathPoint::PathPoint(PointType type) : m_Type(type) {}
PathPoint::PathPoint(PointType type, Vec2D translation) : m_Type(type), m_Translation(translation) {}

PathPoint::~PathPoint() {}

PointType PathPoint::type() const { return m_Type; }

void PathPoint::copy(const PathPoint* from)
{
	m_Type = from->m_Type;
	Vec2D::copy(m_Translation, from->m_Translation);
	m_Weights = std::vector<float>(from->m_Weights);
}

void PathPoint::setWeightCount(int count) { m_Weights.resize(count); }

void PathPoint::read(BlockReader* reader, bool isConnectedToBones)
{
	reader->read(m_Translation);
	readPoint(reader, isConnectedToBones);
	if (!m_Weights.empty())
	{
		reader->readFloat32Array(&m_Weights[0], m_Weights.size());
	}
}

PathPoint* PathPoint::transformed(const Mat2D& transform)
{
	PathPoint* result = makeInstance();
	Vec2D::transform(result->m_Translation, result->m_Translation, transform);
	return result;
}