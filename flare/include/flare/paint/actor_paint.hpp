#ifndef _FLARE_ACTOR_PAINT_HPP_
#define _FLARE_ACTOR_PAINT_HPP_

#include "flare/actor_component.hpp"

namespace flare
{
    class BlockReader;
    class ActorArtboard;
    class ActorShape;

	class ActorPaint : public ActorComponent
	{
		typedef ActorComponent Base;

	protected:
		float m_Opacity;
		float m_RenderOpacity;
		bool m_IsPaintInvalid;
        ActorShape* m_Shape;

    protected:
        void markPaintDirty();
        void invalidatePaint();
        void onParentChanged(ActorNode* from, ActorNode* to) override;
        virtual void onShapeChanged(ActorShape* from, ActorShape* to) = 0;
        
	public:
        ActorPaint(ComponentType type);
        float opacity() const { return m_Opacity; }
        float renderOpacity() const { return m_RenderOpacity; }
        void opacity(float value);
        ActorShape* shape() const { return m_Shape; }

        void copy(const ActorPaint* paint, ActorArtboard* artboard);
        static ActorPaint* read(ActorArtboard* artboard, BlockReader* reader, ActorPaint* component);
        void completeResolve() override;
        void update(DirtyFlags dirt) override;
        virtual void validatePaint();
        virtual void updatePaint() {}
        
	};
} // namespace flare

#endif