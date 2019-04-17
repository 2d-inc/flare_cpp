#include "flare_skia/skr_actor_base_path.hpp"
#include "flare/path/actor_base_path.hpp"

using namespace flare;

const SkPath& SkrActorBasePath::path()
{
	if (!m_IsPathValid)
	{
		updatePath();
		m_IsPathValid = true;
	}

	return m_Path;
}

SkrActorBasePath::SkrActorBasePath(ActorBasePath* path) : m_BasePath(path), m_IsPathValid(false) {}

void SkrActorBasePath::updatePath()
{
	m_Path.rewind();

	const std::vector<PathPoint*>& pts = m_BasePath->deformedPoints();

	if (pts.size() != 0)
	{
		std::vector<PathPoint*> renderPoints;
		std::vector<PathPoint*> cleanup;
		int pl = pts.size();

		const float arcConstant = 0.55f;
		const float iarcConstant = 1.0f - arcConstant;
		bool isClosed = m_BasePath->isClosed();
		PathPoint* previous = isClosed ? pts[pl - 1] : nullptr;
		for (int i = 0; i < pl; i++)
		{
			PathPoint* point = pts[i];
			switch (point->type())
			{
				case PointType::Straight:
				{
					StraightPathPoint* straightPoint = reinterpret_cast<StraightPathPoint*>(point);
					float radius = straightPoint->radius();
					if (radius > 0.0f)
					{
						if (!isClosed && (i == 0 || i == pl - 1))
						{
							renderPoints.push_back(point);
							previous = point;
						}
						else
						{
							PathPoint* next = pts[(i + 1) % pl];
							const Vec2D& prevPoint = previous->type() == PointType::Straight
							                             ? previous->translation()
							                             : reinterpret_cast<CubicPathPoint*>(previous)->outPoint();
							const Vec2D& nextPoint = next->type() == PointType::Straight
							                             ? next->translation()
							                             : reinterpret_cast<CubicPathPoint*>(next)->inPoint();
							const Vec2D& pos = point->translation();

							Vec2D toPrev;
							Vec2D::subtract(toPrev, prevPoint, pos);
							float toPrevLength = Vec2D::length(toPrev);
							toPrev[0] /= toPrevLength;
							toPrev[1] /= toPrevLength;

							Vec2D toNext;
							Vec2D::subtract(toNext, nextPoint, pos);
							float toNextLength = Vec2D::length(toNext);
							toNext[0] /= toNextLength;
							toNext[1] /= toNextLength;

							float renderRadius = std::min(toPrevLength, std::min(toNextLength, radius));

							{
								Vec2D translation, outPoint;
								Vec2D::scaleAndAdd(translation, pos, toPrev, renderRadius);
								Vec2D::scaleAndAdd(outPoint, pos, toPrev, iarcConstant * renderRadius);

								CubicPathPoint* newPoint = new CubicPathPoint(translation, translation, outPoint);
								cleanup.push_back(newPoint);
								renderPoints.push_back(newPoint);
							}

							{
								Vec2D translation, inPoint;
								Vec2D::scaleAndAdd(translation, pos, toNext, renderRadius);
								Vec2D::scaleAndAdd(translation, pos, toNext, iarcConstant * renderRadius);

								CubicPathPoint* newPoint = new CubicPathPoint(translation, inPoint, translation);
								cleanup.push_back(newPoint);
								renderPoints.push_back(newPoint);

								previous = newPoint;
							}
						}
					}
					else
					{
						renderPoints.push_back(point);
						previous = point;
					}
					break;
				}
				default:
					renderPoints.push_back(point);
					previous = point;
					break;
			}
		}

		PathPoint* firstPoint = renderPoints[0];
		Vec2D& translation = firstPoint->translation();
		m_Path.moveTo(translation[0], translation[1]);
		for (int i = 0, l = isClosed ? renderPoints.size() : renderPoints.size() - 1, pl = renderPoints.size(); i < l;
		     i++)
		{
			PathPoint* point = renderPoints[i];
			PathPoint* nextPoint = renderPoints[(i + 1) % pl];
			Vec2D* cin = nextPoint->type() != PointType::Straight ? &(reinterpret_cast<CubicPathPoint*>(nextPoint)->inPoint()) : nullptr;
			Vec2D* cout = point->type() != PointType::Straight ? &(reinterpret_cast<CubicPathPoint*>(point)->outPoint()) : nullptr;
			
			if (cin == nullptr && cout == nullptr)
			{
				Vec2D& translation = nextPoint->translation();
				m_Path.lineTo(translation[0], translation[1]);
			}
			else
			{
				if(cout == nullptr)
				{
					cout = &point->translation();
				}
				if(cin == nullptr)
				{
					cin = &nextPoint->translation();
				}

				Vec2D& fcin = *cin;
				Vec2D& fcout = *cout;
				Vec2D& translation = nextPoint->translation();
				m_Path.cubicTo(fcout[0], fcout[1], fcin[0], fcin[1], translation[0], translation[1]);
			}
		}

		if (isClosed)
		{
			m_Path.close();
		}
		for (PathPoint* point : cleanup)
		{
			delete point;
		}
	}
}