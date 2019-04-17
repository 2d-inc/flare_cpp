#ifndef _FLARE_ACTORSHAPE_HPP_
#define _FLARE_ACTORSHAPE_HPP_

#include "actor_drawable.hpp"
#include <vector>

namespace flare
{
	class ActorStroke;
	class ActorFill;

	class ActorShape : public ActorDrawable
	{
		typedef ActorDrawable Base;

	protected:
		std::vector<ActorStroke*> m_Strokes;
		std::vector<ActorFill*> m_Fills;

	public:
		ActorShape();
		~ActorShape();
		virtual void invalidateDrawable();
		void update(DirtyFlags dirt) override;
		void initializeGraphics() override;

		void addStroke(ActorStroke* stroke);
		void addFill(ActorFill* fill);

		void removeStroke(ActorStroke* stroke);
		void removeFill(ActorFill* fill);

	protected:
		virtual void onStrokesChanged() {}
		virtual void onFillsChanged() {}
	};
} // namespace flare
#endif