#ifndef _FLARE_TRIM_PATH_HPP_
#define _FLARE_TRIM_PATH_HPP_

namespace flare
{
	enum class TrimPath : signed int
	{
		Off = 0,
		Sequential = 1,
        Synced = 2
	};
}

#endif