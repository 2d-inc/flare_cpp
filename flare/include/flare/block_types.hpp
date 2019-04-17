#ifndef _FLARE_BLOCKTYPE_HPP_
#define _FLARE_BLOCKTYPE_HPP_
namespace flare
{
	enum class BlockType
	{
		Unknown = 0,
		Components = 1,
		ActorNode = 2,
		ActorBone = 3,
		ActorRootBone = 4,
		ActorImage = 5,
		View = 6,
		Animation = 7,
		Animations = 8,
		Atlases = 9,
		Atlas = 10,
		ActorIKTarget = 11,
		ActorEvent = 12,
		CustomIntProperty = 13,
		CustomFloatProperty = 14,
		CustomStringProperty = 15,
		CustomBooleanProperty = 16,
		ColliderRectangle = 17,  // TODO
		ColliderTriangle = 18,   // TODO
		ColliderCircle = 19,     // TODO
		ColliderPolygon = 20,    // TODO
		ColliderLine = 21,       // TODO
		ActorImageSequence = 22, // TODO
		ActorNodeSolo = 23,
		NestedActorNode = 24,
		NestedActorAssets = 25,
		NestedActorAsset = 26,
		ActorStaticMesh = 27,
		JellyComponent = 28,
		ActorJellyBone = 29,
		ActorIKConstraint = 30,
		ActorDistanceConstraint = 31,
		ActorTranslationConstraint = 32,
		ActorRotationConstraint = 33,
		ActorScaleConstraint = 34,
		ActorTransformConstraint = 35,
		ActorShape = 100,
		ActorPath = 101,
		ColorFill = 102,
		ColorStroke = 103,
		GradientFill = 104,
		GradientStroke = 105,
		RadialGradientFill = 106,
		RadialGradientStroke = 107,
		ActorEllipse = 108,
		ActorRectangle = 109,
		ActorTriangle = 110,
		ActorStar = 111,
		ActorPolygon = 112,
		ActorSkin = 113,
		ActorArtboard = 114,
		Artboards = 115
	};
}
#endif