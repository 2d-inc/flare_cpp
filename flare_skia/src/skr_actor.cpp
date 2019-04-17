#include "flare_skia/skr_actor.hpp"
#include "flare_skia/skr_actor_artboard.hpp"
#include "flare_skia/skr_actor_ellipse.hpp"
#include "flare_skia/skr_actor_path.hpp"
#include "flare_skia/skr_actor_polygon.hpp"
#include "flare_skia/skr_actor_rectangle.hpp"
#include "flare_skia/skr_actor_shape.hpp"
#include "flare_skia/skr_actor_star.hpp"
#include "flare_skia/skr_actor_triangle.hpp"
#include "flare_skia/skr_color_fill.hpp"
#include "flare_skia/skr_color_stroke.hpp"
#include "flare_skia/skr_drawable_proxy.hpp"
#include "flare_skia/skr_gradient_fill.hpp"
#include "flare_skia/skr_gradient_stroke.hpp"
#include "flare_skia/skr_radial_gradient_fill.hpp"
#include "flare_skia/skr_radial_gradient_stroke.hpp"

using namespace flare;

ActorArtboard* SkrActor::makeArtboard() { return new SkrActorArtboard(this); }

ActorImage* SkrActor::makeImageNode() { return Base::makeImageNode(); }

ActorStaticMesh* SkrActor::makeStaticMeshNode() { return Base::makeStaticMeshNode(); }

ActorShape* SkrActor::makeShape() { return new SkrActorShape(); }

ActorColorFill* SkrActor::makeColorFill() { return new SkrColorFill(); }

ActorColorStroke* SkrActor::makeColorStroke() { return new SkrColorStroke(); }

ActorGradientFill* SkrActor::makeGradientFill() { return new SkrGradientFill(); }

ActorGradientStroke* SkrActor::makeGradientStroke() { return new SkrGradientStroke(); }

ActorRadialGradientFill* SkrActor::makeRadialGradientFill() { return new SkrRadialGradientFill(); }

ActorRadialGradientStroke* SkrActor::makeRadialGradientStroke() { return new SkrRadialGradientStroke(); }

ActorPath* SkrActor::makePath() { return new SkrActorPath(); }

ActorRectangle* SkrActor::makeRectangle() { return new SkrActorRectangle(); }

ActorTriangle* SkrActor::makeTriangle() { return new SkrActorTriangle(); }

ActorPolygon* SkrActor::makePolygon() { return new SkrActorPolygon(); }

ActorStar* SkrActor::makeStar() { return new SkrActorStar(); }

ActorEllipse* SkrActor::makeEllipse() { return new SkrActorEllipse(); }

ActorDrawableProxy* SkrActor::makeDrawableProxy(ActorDrawable* drawable) { return new SkrDrawableProxy(drawable); }