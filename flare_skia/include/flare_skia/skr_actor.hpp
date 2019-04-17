#ifndef _SKR_ACTOR_HPP_
#define _SKR_ACTOR_HPP_

#include "flare/actor.hpp"

namespace flare
{
	class SkrActor : public Actor
	{
		typedef Actor Base;

	public:
		ActorArtboard* makeArtboard() override;
		ActorImage* makeImageNode() override;
		ActorStaticMesh* makeStaticMeshNode() override;
		ActorShape* makeShape() override;
		ActorColorFill* makeColorFill() override;
		ActorColorStroke* makeColorStroke() override;
		ActorGradientFill* makeGradientFill() override;
		ActorGradientStroke* makeGradientStroke() override;
		ActorRadialGradientFill* makeRadialGradientFill() override;
		ActorRadialGradientStroke* makeRadialGradientStroke() override;
		ActorPath* makePath() override;
		ActorRectangle* makeRectangle() override;
		ActorTriangle* makeTriangle() override;
		ActorPolygon* makePolygon() override;
		ActorStar* makeStar() override;
		ActorEllipse* makeEllipse() override;
		ActorDrawableProxy* makeDrawableProxy(ActorDrawable* drawable) override;
	};
} // namespace flare
#endif