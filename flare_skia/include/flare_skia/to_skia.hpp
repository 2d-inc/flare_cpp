#ifndef _TO_SKIA_HPP_
#define _TO_SKIA_HPP_

#include "SkPaint.h"
#include "flare/blend_mode.hpp"
#include "flare/paint/stroke_cap.hpp"
#include "flare/paint/stroke_join.hpp"
#include "flare_math/color.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_math/vec2d.hpp"

namespace flare
{
	class ToSkia
	{
	public:
		static SkMatrix convert(const flare::Mat2D& m)
		{
			SkMatrix skMatrix;
			skMatrix.set9((SkScalar[9])
			              // Skia Matrix is row major
			              {// Row 1
			               m[0], m[2], m[4],
			               // Row 2
			               m[1], m[3], m[5],
			               // Row 3
			               0.0, 0.0, 1.0});
			return skMatrix;
		}

		static SkPoint convert(const flare::Vec2D& point) { return SkPoint::Make(point[0], point[1]); }

		static SkColor4f convert(const flare::Color& color)
		{
			return SkColor4f{color[0], color[1], color[2], color[3]};
		}

		static SkBlendMode convert(flare::BlendMode blendMode)
		{
			switch (blendMode)
			{
				case flare::BlendMode::Clear:
					return SkBlendMode::kClear;
				case flare::BlendMode::Src:
					return SkBlendMode::kSrc;
				case flare::BlendMode::Dst:
					return SkBlendMode::kDst;
				case flare::BlendMode::SrcOver:
					return SkBlendMode::kSrcOver;
				case flare::BlendMode::DstOver:
					return SkBlendMode::kDstOver;
				case flare::BlendMode::SrcIn:
					return SkBlendMode::kSrcIn;
				case flare::BlendMode::DstIn:
					return SkBlendMode::kDstIn;
				case flare::BlendMode::SrcOut:
					return SkBlendMode::kSrcOut;
				case flare::BlendMode::DstOut:
					return SkBlendMode::kDstOut;
				case flare::BlendMode::SrcATop:
					return SkBlendMode::kSrcATop;
				case flare::BlendMode::DstATop:
					return SkBlendMode::kDstATop;
				case flare::BlendMode::Xor:
					return SkBlendMode::kXor;
				case flare::BlendMode::Plus:
					return SkBlendMode::kPlus;
				case flare::BlendMode::Modulate:
					return SkBlendMode::kModulate;
				case flare::BlendMode::Screen:
					return SkBlendMode::kScreen;
				case flare::BlendMode::Overlay:
					return SkBlendMode::kOverlay;
				case flare::BlendMode::Darken:
					return SkBlendMode::kDarken;
				case flare::BlendMode::Lighten:
					return SkBlendMode::kLighten;
				case flare::BlendMode::ColorDodge:
					return SkBlendMode::kColorDodge;
				case flare::BlendMode::ColorBurn:
					return SkBlendMode::kColorBurn;
				case flare::BlendMode::HardLight:
					return SkBlendMode::kHardLight;
				case flare::BlendMode::SoftLight:
					return SkBlendMode::kSoftLight;
				case flare::BlendMode::Difference:
					return SkBlendMode::kDifference;
				case flare::BlendMode::Exclusion:
					return SkBlendMode::kExclusion;
				case flare::BlendMode::Multiply:
					return SkBlendMode::kMultiply;
				case flare::BlendMode::Hue:
					return SkBlendMode::kHue;
				case flare::BlendMode::Saturation:
					return SkBlendMode::kSaturation;
				case flare::BlendMode::Color:
					return SkBlendMode::kColor;
				case flare::BlendMode::Luminosity:
					return SkBlendMode::kLuminosity;
			}
			return SkBlendMode::kSrcOver;
		}

		static SkPaint::Cap convert(flare::StrokeCap cap)
		{
			switch (cap)
			{
				case flare::StrokeCap::Butt:
					return SkPaint::Cap::kButt_Cap;
				case flare::StrokeCap::Round:
					return SkPaint::Cap::kRound_Cap;
				case flare::StrokeCap::Square:
					return SkPaint::Cap::kSquare_Cap;
			}
			return SkPaint::Cap::kDefault_Cap;
		}

		static SkPaint::Join convert(flare::StrokeJoin join)
		{
			switch (join)
			{
				case flare::StrokeJoin::Round:
					return SkPaint::Join::kRound_Join;
				case flare::StrokeJoin::Bevel:
					return SkPaint::Join::kBevel_Join;
				case flare::StrokeJoin::Miter:
					return SkPaint::Join::kMiter_Join;
			}
			return SkPaint::Join::kDefault_Join;
		}
	};
} // namespace flare
#endif