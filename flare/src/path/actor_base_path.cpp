#include "flare/path/actor_base_path.hpp"
#include "flare/actor_shape.hpp"

using namespace flare;

ActorBasePath::ActorBasePath(ComponentType type) : Base(type), m_Shape(nullptr) {}

void ActorBasePath::onParentChanged(ActorNode* from, ActorNode* to)
{
	m_Shape = dynamic_cast<ActorShape*>(to);
}

AABB ActorBasePath::getPathAABB()
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();

	AABB obb = getPathOBB();
	Vec2D pts[4] = {Vec2D(obb[0], obb[1]), Vec2D(obb[2], obb[1]), Vec2D(obb[2], obb[3]), Vec2D(obb[0], obb[3])};
	Mat2D localTransform;
	if (isPathInWorldSpace())
	{
		//  convert the path coordinates into local parent space.
		Mat2D::invert(localTransform, parent()->worldTransform());
	}
	else
	{
		localTransform = transform();
	}

	for (Vec2D& p : pts)
	{
		Vec2D::transform(p, p, localTransform);
		if (p[0] < minX)
		{
			minX = p[0];
		}
		if (p[1] < minY)
		{
			minY = p[1];
		}

		if (p[0] > maxX)
		{
			maxX = p[0];
		}
		if (p[1] > maxY)
		{
			maxY = p[1];
		}
	}
	return AABB(minX, minY, maxX, maxY);
}

AABB ActorBasePath::getPathOBB()
{
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();

	const std::vector<PathPoint*> renderPoints = points();
	for (PathPoint* point : renderPoints)
	{
		Vec2D translation = point->translation();
		float x = translation[0];
		float y = translation[1];

		if (x < minX)
		{
			minX = x;
		}
		if (y < minY)
		{
			minY = y;
		}
		if (x > maxX)
		{
			maxX = x;
		}
		if (y > maxY)
		{
			maxY = y;
		}

		if (point->type() != PointType::Straight)
		{
			CubicPathPoint* cubicPoint = reinterpret_cast<CubicPathPoint*>(point);
			Vec2D t = cubicPoint->inPoint();
			x = t[0];
			y = t[1];
			if (x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}

			t = cubicPoint->outPoint();
			x = t[0];
			y = t[1];
			if (x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
		}
	}

	return AABB(minX, minY, maxX, maxY);
}

void ActorBasePath::invalidateDrawable() {}