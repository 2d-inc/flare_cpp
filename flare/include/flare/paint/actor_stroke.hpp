#ifndef _FLARE_ACTOR_STROKE_HPP_
#define _FLARE_ACTOR_STROKE_HPP_

#include "flare/paint/stroke_cap.hpp"
#include "flare/paint/stroke_join.hpp"
#include "flare/paint/trim_path.hpp"

namespace flare
{
	class BlockReader;
	class ActorArtboard;

	class ActorStroke
	{
	private:
		float m_Width;
		StrokeCap m_Cap;
		StrokeJoin m_Join;
		TrimPath m_TrimPath;
		float m_TrimStart;
		float m_TrimEnd;
		float m_TrimOffset;

	protected:
		ActorStroke();

	public:
		float width() const { return m_Width; }
		void width(float value);

		StrokeCap cap() const { return m_Cap; }
		void cap(StrokeCap value);

		StrokeJoin join() const { return m_Join; }
		void join(StrokeJoin value);

		TrimPath trimPath() const { return m_TrimPath; }
		void trimPath(TrimPath value);

		float trimStart() const { return m_TrimStart; }
		bool trimStart(float value);

		float trimEnd() const { return m_TrimEnd; }
		bool trimEnd(float value);

		float trimOffset() const { return m_TrimOffset; }
		bool trimOffset(float value);

		void copy(const ActorStroke* stroke, ActorArtboard* artboard);
		static ActorStroke* read(ActorArtboard* artboard, BlockReader* reader, ActorStroke* component);

		virtual void initializeGraphics() = 0;
		virtual void markPaintDirty() = 0;
		virtual void markPathEffectsDirty() = 0;
		virtual void validatePaint() = 0;
	};
} // namespace flare

#endif