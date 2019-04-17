#ifndef _FLARE_FILL_RULE_HPP_
#define _FLARE_FILL_RULE_HPP_

namespace flare
{
	enum class FillRule : signed int
	{
		EvenOdd = 0,
		NonZero = 1
	};
}

#endif