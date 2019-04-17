#ifndef _FLARE_ACTORPATH_HPP_
#define _FLARE_ACTORPATH_HPP_

#include "flare_math/aabb.hpp"
#include "flare_math/mat2d.hpp"
#include "flare/actor_skinnable.hpp"
#include "flare/path/actor_base_path.hpp"
#include "flare/path/path_point.hpp"
#include <vector>

namespace flare
{
	class ActorPath : public ActorBasePath, public ActorSkinnable
	{
		typedef ActorBasePath Base;

	private:
		bool m_IsHidden;
		bool m_IsClosed;
		std::vector<PathPoint*> m_Points;
        std::vector<PathPoint*> m_DeformedPoints;
        float* m_VertexDeform;
        int m_VertexDeformLength;

		static const unsigned char VertexDeformDirty = 1 << 2;
	public:
		ActorPath();
		~ActorPath();

		bool isPathInWorldSpace() const override { return isConnectedToBones(); }
		const Mat2D& pathTransform() const override;
		const std::vector<PathPoint*>& deformedPoints() override;
		const std::vector<PathPoint*>& points() override { return m_Points; };
		float* vertexDeform() { return m_VertexDeform; }
		void onDirty(DirtyFlags dirt) override;

		void copy(const ActorPath* path, ActorArtboard* resetActor);
		void resolveComponentIndices(ActorComponent** nodes, int numComponents) override;
		void invalidateDrawable() override;
        static ActorPath* read(ActorArtboard* artboard, BlockReader* reader, ActorPath* component = NULL);
        void makeVertexDeform();
        void markVertexDeformDirty();
        void update(DirtyFlags dirt) override;

        // Help ActorSkinnable find the override call.
		void overrideWorldTransform(const Mat2D& transform) override { ActorNode::overrideWorldTransform(transform); }

		bool isClosed() override { return m_IsClosed; }
	};
} // namespace flare
#endif