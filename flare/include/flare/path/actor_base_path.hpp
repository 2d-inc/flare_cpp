#ifndef _FLARE_ACTORBASEPATH_HPP_
#define _FLARE_ACTORBASEPATH_HPP_

#include "flare_math/mat2d.hpp"
#include "flare_math/aabb.hpp"
#include "flare/actor_drawable.hpp"
#include "flare/path/path_point.hpp"
#include "flare/actor_node.hpp"
#include <vector>

namespace flare
{
    class ActorShape;
	class ActorBasePath : public ActorNode
	{
        typedef ActorNode Base;
    private:
        ActorShape* m_Shape;
    protected:
        void onParentChanged(ActorNode* from, ActorNode* to) override;
        
	public:
		ActorBasePath(ComponentType type);

        virtual bool isPathInWorldSpace() const = 0;
        const virtual Mat2D& pathTransform() const = 0;
        virtual const std::vector<PathPoint*>& points() = 0;
        virtual const std::vector<PathPoint*>& deformedPoints() = 0;
        virtual bool isClosed() = 0;

        AABB getPathAABB();
        AABB getPathOBB();
        virtual void invalidateDrawable();
        ActorShape* shape() const { return m_Shape; }
	};
} // namespace flare
#endif