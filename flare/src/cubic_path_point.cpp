#include "flare/block_reader.hpp"
#include "flare/path/path_point.hpp"

using namespace flare;

CubicPathPoint::CubicPathPoint(PointType type) : PathPoint(type) {}

CubicPathPoint::CubicPathPoint(Vec2D translation, Vec2D inPoint, Vec2D outPoint) :
    PathPoint(PointType::Disconnected, translation),
    m_In(inPoint),
    m_Out(outPoint)
{
}

PathPoint* CubicPathPoint::makeInstance() const
{
	CubicPathPoint* point = new CubicPathPoint(type());
	point->copy(this);
	return point;
}

void CubicPathPoint::copy(const CubicPathPoint* from)
{
	PathPoint::copy(from);
	Vec2D::copy(m_In, from->m_In);
	Vec2D::copy(m_Out, from->m_Out);
}

void CubicPathPoint::readPoint(BlockReader* reader, bool isConnectedToBones)
{
	reader->read(m_In);
	reader->read(m_Out);
	if (isConnectedToBones)
	{
		setWeightCount(24);
	}
}

PathPoint* CubicPathPoint::transformed(const Mat2D& transform)
{
	CubicPathPoint* result = reinterpret_cast<CubicPathPoint*>(PathPoint::transformed(transform));
	Vec2D::transform(result->m_In, result->m_In, transform);
	Vec2D::transform(result->m_Out, result->m_Out, transform);
	return result;
}

PathPoint* CubicPathPoint::skin(const Mat2D& world, const float* bones)
{
	CubicPathPoint* point = new CubicPathPoint(type());

	float px = world[0] * m_Translation[0] + world[2] * m_Translation[1] + world[4];
	float py = world[1] * m_Translation[0] + world[3] * m_Translation[1] + world[5];
	{
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
	}

	{
		float a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0, f = 0.0;
		px = world[0] * m_In[0] + world[2] * m_In[1] + world[4];
		py = world[1] * m_In[0] + world[3] * m_In[1] + world[5];

		for (int i = 8; i < 12; i++)
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

		Vec2D& pos = point->m_In;
		pos[0] = a * px + c * py + e;
		pos[1] = b * px + d * py + f;
	}

	{
		float a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0, f = 0.0;
		px = world[0] * m_Out[0] + world[2] * m_Out[1] + world[4];
		py = world[1] * m_Out[0] + world[3] * m_Out[1] + world[5];

		for (int i = 16; i < 20; i++)
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

		Vec2D& pos = point->m_Out;
		pos[0] = a * px + c * py + e;
		pos[1] = b * px + d * py + f;
	}

	return point;
}