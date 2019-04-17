#ifndef _FLARE_DIRTY_FLAGS_HPP_
#define _FLARE_DIRTY_FLAGS_HPP_

#include <type_traits>

namespace flare
{
	enum class DirtyFlags : unsigned char
	{
		None = 0,
		TransformDirty = 1 << 0,
		WorldTransformDirty = 1 << 1,
		PaintDirty = 1 << 2,
		VertexDeformDirty = 1 << 3,
		ChildrenDirty = 1 << 4,
		Filty = 255
	};

	inline constexpr DirtyFlags operator&(DirtyFlags lhs, DirtyFlags rhs)
	{
		return static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) &
		                               static_cast<std::underlying_type<DirtyFlags>::type>(rhs));
	}

	inline constexpr DirtyFlags operator^(DirtyFlags lhs, DirtyFlags rhs)
	{
		return static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) ^
		                               static_cast<std::underlying_type<DirtyFlags>::type>(rhs));
	}

	inline constexpr DirtyFlags operator|(DirtyFlags lhs, DirtyFlags rhs)
	{
		return static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) |
		                               static_cast<std::underlying_type<DirtyFlags>::type>(rhs));
	}

	inline constexpr DirtyFlags operator~(DirtyFlags rhs)
	{
		return static_cast<DirtyFlags>(~static_cast<std::underlying_type<DirtyFlags>::type>(rhs));
	}

	inline DirtyFlags& operator|=(DirtyFlags& lhs, DirtyFlags rhs)
	{
		lhs = static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) |
		                              static_cast<std::underlying_type<DirtyFlags>::type>(rhs));

		return lhs;
	}

	inline DirtyFlags& operator&=(DirtyFlags& lhs, DirtyFlags rhs)
	{
		lhs = static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) &
		                              static_cast<std::underlying_type<DirtyFlags>::type>(rhs));

		return lhs;
	}

	inline DirtyFlags& operator^=(DirtyFlags& lhs, DirtyFlags rhs)
	{
		lhs = static_cast<DirtyFlags>(static_cast<std::underlying_type<DirtyFlags>::type>(lhs) ^
		                              static_cast<std::underlying_type<DirtyFlags>::type>(rhs));

		return lhs;
	}
} // namespace flare

#endif