#ifndef _FLARE_PATHPOINT_HPP_
#define _FLARE_PATHPOINT_HPP_

#include "flare_math/mat2d.hpp"
#include "flare_math/vec2d.hpp"
#include <vector>

namespace flare
{
	class BlockReader;

	enum class PointType : unsigned char
	{
		Straight = 0,
		Mirror = 1,
		Disconnected = 2,
		Asymmetric = 3
	};

	class PathPoint
	{
	private:
		PointType m_Type;

	protected:
		Vec2D m_Translation;
		std::vector<float> m_Weights;
		void setWeightCount(int count);

	public:
		PathPoint(PointType type);
		PathPoint(PointType type, Vec2D translation);
		virtual ~PathPoint();
		PointType type() const;

		Vec2D& translation() { return m_Translation; }
		virtual PathPoint* makeInstance() const = 0;
		void copy(const PathPoint* from);
		void read(BlockReader* reader, bool isConnectedToBones);
		virtual void readPoint(BlockReader* reader, bool isConnectedToBones) = 0;
		virtual PathPoint* transformed(const Mat2D& transform);
		virtual PathPoint* skin(const Mat2D& world, const float* bones) = 0;
	};

	class StraightPathPoint : public PathPoint
	{
	private:
		float m_Radius;

	public:
		StraightPathPoint();
		StraightPathPoint(Vec2D translation);
		StraightPathPoint(Vec2D translation, float radius);
		PathPoint* makeInstance() const override;
		float radius() const { return m_Radius; }
		void radius(float value);
		void copy(const StraightPathPoint* from);
		void readPoint(BlockReader* reader, bool isConnectedToBones) override;
		PathPoint* skin(const Mat2D& world, const float* bones) override;
	};

	class CubicPathPoint : public PathPoint
	{
	private:
		Vec2D m_In;
		Vec2D m_Out;

	public:
		CubicPathPoint(PointType type);
		CubicPathPoint(Vec2D translation, Vec2D inPoint, Vec2D outPoint);
		PathPoint* makeInstance() const override;
		void copy(const CubicPathPoint* from);
		void readPoint(BlockReader* reader, bool isConnectedToBones) override;
		PathPoint* skin(const Mat2D& world, const float* bones) override;
		PathPoint* transformed(const Mat2D& transform) override;

		Vec2D& inPoint() { return m_In; }
		Vec2D& outPoint() { return m_Out; }
	};
} // namespace flare
#endif