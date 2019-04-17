#ifndef _FLARE_ACTORDRAWABLE_HPP_
#define _FLARE_ACTORDRAWABLE_HPP_

#include "actor_node.hpp"
#include "blend_mode.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorDrawable : public ActorNode
	{
		typedef ActorNode Base;

	protected:
		// TODO: implement clips.
		int m_DrawOrder;
		bool m_IsHidden;
		BlendMode m_BlendMode;
		virtual void onBlendModeChanged(BlendMode from, BlendMode to);

	public:
		ActorDrawable(ComponentType type);
		void copy(const ActorDrawable* node, ActorArtboard* artboard);
		int drawOrder() const;
		void drawOrder(int order);
		bool isHidden() const;
		void isHidden(bool value);
		BlendMode blendMode() const;
		void blendMode(BlendMode value);
		static ActorDrawable* read(ActorArtboard* artboard, BlockReader* reader, ActorDrawable* component);
		virtual void initializeGraphics() = 0;

		bool doesDraw() const { return !m_IsHidden && !renderCollapsed(); }
	};
} // namespace flare
#endif