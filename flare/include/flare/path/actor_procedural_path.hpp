#ifndef _FLARE_ACTORPROCEDURALPATH_HPP_
#define _FLARE_ACTORPROCEDURALPATH_HPP_

#include "flare/actor_drawable.hpp"
#include "flare/path/actor_base_path.hpp"
#include "flare/path/path_point.hpp"
#include "flare_math/aabb.hpp"
#include "flare_math/mat2d.hpp"
#include <vector>

namespace flare
{
	class ActorProceduralPath : public ActorBasePath
	{
		typedef ActorBasePath Base;

	private:
		float m_Width;
		float m_Height;

	protected:
		std::vector<PathPoint*> m_Points;

	public:
		ActorProceduralPath(ComponentType type);
		~ActorProceduralPath();

		float width() const { return m_Width; }
		float height() const { return m_Height; }

		void width(float value);
		void height(float value);

		bool isPathInWorldSpace() const override { return false; }
		const Mat2D& pathTransform() const override { return worldTransform(); }
		const std::vector<PathPoint*>& deformedPoints() override;
		const std::vector<PathPoint*>& points() override;
		void onDirty(DirtyFlags dirt) override;

		void copy(const ActorProceduralPath* path, ActorArtboard* resetActor);
		void invalidateDrawable() override;
		virtual void makePoints() = 0;
		bool isClosed() override { return true; }

		static ActorProceduralPath* read(ActorArtboard* artboard, BlockReader* reader, ActorProceduralPath* component);
	};
} // namespace flare
#endif