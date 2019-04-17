#ifndef _FLARE_PROPERTYANIMATION_HPP_
#define _FLARE_PROPERTYANIMATION_HPP_

#include <string>
#include <vector>

namespace flare
{
	class BlockReader;
	class ActorComponent;
	class KeyFrame;
	enum class PropertyType
	{
		Unknown = 0,
		PosX = 1,
		PosY = 2,
		ScaleX = 3,
		ScaleY = 4,
		Rotation = 5,
		Opacity = 6,
		DrawOrder = 7,
		Length = 8,
		ImageVertices = 9,
		ConstraintStrength = 10,
		Trigger = 11,
		IntProperty = 12,
		FloatProperty = 13,
		StringProperty = 14,
		BooleanProperty = 15,
		IsCollisionEnabled = 16,
		Sequence = 17,
		ActiveChildIndex = 18,
		PathVertices = 19,
		FillColor = 20,
		FillGradient = 21,
		FillRadial = 22,
		StrokeColor = 23,
		StrokeGradient = 24,
		StrokeRadial = 25,
		StrokeWidth = 26,
		StrokeOpacity = 27,
		FillOpacity = 28,
		ShapeWidth = 29,
		ShapeHeight = 30,
		CornerRadius = 31,
		InnerRadius = 32,
		StrokeStart = 33,
		StrokeEnd = 34,
		StrokeOffset = 35,
		Max
	};

	class PropertyAnimation
	{
	private:
		PropertyType m_Type;
		std::vector<KeyFrame*> m_KeyFrames;

	public:
		PropertyAnimation();
		~PropertyAnimation();

		int keyFramesCount() const;
		inline const KeyFrame* keyFrame(int idx) const { return m_KeyFrames[idx]; }

		PropertyType type() const;
		void read(BlockReader* reader, ActorComponent* component);
		void apply(float time, ActorComponent* component, float mix);
	};
} // namespace flare
#endif