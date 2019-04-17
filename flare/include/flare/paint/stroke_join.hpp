#ifndef _FLARE_STROKE_JOIN_HPP_
#define _FLARE_STROKE_JOIN_HPP_

namespace flare
{
	enum class StrokeJoin : signed int
	{
		Miter = 0,
		Round = 1,
        Bevel = 2
	};
}

#endif