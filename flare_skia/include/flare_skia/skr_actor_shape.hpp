#ifndef _SKR_ACTOR_SHAPE_HPP_
#define _SKR_ACTOR_SHAPE_HPP_

#include "flare/actor_shape.hpp"
#include "skr_drawable.hpp"
#include "SkPath.h"
#include <vector>

namespace flare
{
	class SkrActorBasePath;
	class SkrFill;
	class SkrStroke;

	class SkrActorShape : public ActorShape, public SkrDrawable
	{
		typedef ActorShape Base;

	private:
		bool m_IsValid;
		SkPath m_Path;
		std::vector<SkrActorBasePath*> m_SubPaths;

		// We currently only support rendering a single fill/stroke so we only
		// store these two concrete pointers.
		SkrStroke* m_Stroke;
		SkrFill* m_Fill;

	protected:
		void onBlendModeChanged(BlendMode from, BlendMode to) override;
		void onRenderOpacityChanged() override;
		void onChildrenChanged() override;

	public:
		SkrActorShape();
		void invalidateDrawable() override;

		void draw(SkCanvas* canvas) override;
		SkPath& path();

	protected:
		void onStrokesChanged() override;
		void onFillsChanged() override;
		void onClipsChanged() override { SkrDrawable::updateClips(this); }
	};
} // namespace flare

#endif