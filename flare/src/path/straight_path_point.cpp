#include "flare/block_reader.hpp"
#include "flare/path/path_point.hpp"

using namespace flare;

StraightPathPoint::StraightPathPoint() : PathPoint(PointType::Straight), m_Radius(0.0f) {}
StraightPathPoint::StraightPathPoint(Vec2D translation) : PathPoint(PointType::Straight, translation), m_Radius(0.0f) {}
StraightPathPoint::StraightPathPoint(Vec2D translation, float radius) : PathPoint(PointType::Straight, translation), m_Radius(radius) {}

PathPoint* StraightPathPoint::makeInstance() const
{
	StraightPathPoint* point = new StraightPathPoint();
	point->copy(this);
	return point;
}

void StraightPathPoint::copy(const StraightPathPoint* from)
{
	PathPoint::copy(from);
	m_Radius = from->m_Radius;
}

void StraightPathPoint::radius(float value)
{
    m_Radius = value;
}

void StraightPathPoint::readPoint(BlockReader* reader, bool isConnectedToBones)
{
	m_Radius = reader->readFloat32();
	if (isConnectedToBones)
	{
		setWeightCount(8);
	}
}

PathPoint* StraightPathPoint::skin(const Mat2D& world, const float* bones)
{
	StraightPathPoint* point = new StraightPathPoint();
	point->m_Radius = m_Radius;

	float px = world[0] * m_Translation[0] + world[2] * m_Translation[1] + world[4];
	float py = world[1] * m_Translation[0] + world[3] * m_Translation[1] + world[5];

	float a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0, f = 0.0;

	for (int i = 0; i < 4; i++)
	{
		int boneIndex = (int)m_Weights[i];
		float weight = m_Weights[i + 4];
		if (weight > 0)
		{
			int bb = boneIndex * 6;

			a += bones[bb] * weight;
			b += bones[bb + 1] * weight;
			c += bones[bb + 2] * weight;
			d += bones[bb + 3] * weight;
			e += bones[bb + 4] * weight;
			f += bones[bb + 5] * weight;
		}
	}

	Vec2D& pos = point->translation();
	pos[0] = a * px + c * py + e;
	pos[1] = b * px + d * py + f;

	return point;
}