#ifndef _FLARE_EXAMPLE_LAYOUT_HPP_
#define _FLARE_EXAMPLE_LAYOUT_HPP_

#include "SkCanvas.h"
#include "flare_math/aabb.hpp"
#include <cmath>

namespace flare
{
	enum class Fit
	{
		fill,
		contain,
		cover,
		fitWidth,
		fitHeight,
		none,
		scaleDown
	};

	class Alignment
	{
		float m_X, m_Y;

	public:
		Alignment(float x, float y) : m_X(x), m_Y(y) {}

		float x() const { return m_X; }
		float y() const { return m_Y; }

		static const Alignment topLeft()
		{
			static Alignment align(-1.0f, -1.0f);
			return align;
		}

		static const Alignment topCenter()
		{
			static Alignment align(0.0f, -1.0f);
			return align;
		}

		static const Alignment topRight()
		{
			static Alignment align(1.0f, -1.0f);
			return align;
		}

		static const Alignment centerLeft()
		{
			static Alignment align(-1.0f, 0.0f);
			return align;
		}

		static const Alignment center()
		{
			static Alignment align(0.0f, 0.0f);
			return align;
		}

		static const Alignment centerRight()
		{
			static Alignment align(1.0f, 0.0f);
			return align;
		}

		static const Alignment bottomLeft()
		{
			static Alignment align(-1.0f, 1.0f);
			return align;
		}

		static const Alignment bottomCenter()
		{
			static Alignment align(0.0f, 1.0f);
			return align;
		}

		static const Alignment bottomRight()
		{
			static Alignment align(1.0f, 1.0f);
			return align;
		}

		void transform(SkCanvas* canvas, const AABB& frame, const AABB& content, Fit fit) const
		{
			float contentWidth = content[2] - content[0];
			float contentHeight = content[3] - content[1];
			float x = -content[0] - contentWidth / 2.0 - (m_X * contentWidth / 2.0);
			float y = -content[1] - contentHeight / 2.0 - (m_Y * contentHeight / 2.0);

			float scaleX = 1.0, scaleY = 1.0;

			switch (fit)
			{
				case Fit::fill:
				{
					scaleX = frame.width() / contentWidth;
					scaleY = frame.height() / contentHeight;
					break;
				}
				case Fit::contain:
				{
					float minScale = std::min(frame.width() / contentWidth, frame.height() / contentHeight);
					scaleX = scaleY = minScale;
					break;
				}
				case Fit::cover:
				{
					float maxScale = std::max(frame.width() / contentWidth, frame.height() / contentHeight);
					scaleX = scaleY = maxScale;
					break;
				}
				case Fit::fitHeight:
				{
					float minScale = frame.height() / contentHeight;
					scaleX = scaleY = minScale;
					break;
				}
				case Fit::fitWidth:
				{
					float minScale = frame.width() / contentWidth;
					scaleX = scaleY = minScale;
					break;
				}
				case Fit::none:
				{
					scaleX = scaleY = 1.0;
					break;
				}
				case Fit::scaleDown:
				{
					float minScale = std::min(frame.width() / contentWidth, frame.height() / contentHeight);
					scaleX = scaleY = minScale < 1.0 ? minScale : 1.0;
					break;
				}
			}

			canvas->translate(frame[0] + frame.width() / 2.0 + (m_X * frame.width() / 2.0),
			                  frame[1] + frame.height() / 2.0 + (m_Y * frame.height() / 2.0));

			canvas->scale(scaleX, scaleY);
			canvas->translate(x, y);
		}
	};
} // namespace flare
#endif