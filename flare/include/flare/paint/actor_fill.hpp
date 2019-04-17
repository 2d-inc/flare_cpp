#ifndef _FLARE_ACTOR_FILL_HPP_
#define _FLARE_ACTOR_FILL_HPP_

#include "flare/paint/fill_rule.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorFill
	{
	private:
		FillRule m_FillRule;

	protected:
		ActorFill();

	public:
		FillRule fillRule() const { return m_FillRule; }
		void copy(const ActorFill* fill, ActorArtboard* artboard);
		static ActorFill* read(ActorArtboard* artboard, BlockReader* reader, ActorFill* component);

		virtual void initializeGraphics() = 0;
		virtual void markPaintDirty() = 0;
		virtual void validatePaint() = 0;
	};
} // namespace flare

#endif