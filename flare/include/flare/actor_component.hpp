#ifndef _FLARE_ACTORCOMPONENT_HPP_
#define _FLARE_ACTORCOMPONENT_HPP_

#include "dirty_flags.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_math/vec2d.hpp"
#include <string>
#include <vector>

namespace flare
{
	class ActorArtboard;
	class ActorNode;
	class BlockReader;
	class CustomIntProperty;
	class CustomFloatProperty;
	class CustomStringProperty;
	class CustomBooleanProperty;

	enum class ComponentType
	{
		ActorNode = 2,
		ActorBone = 3,
		ActorRootBone = 4,
		ActorImage = 5,
		ActorIKTarget = 11,
		ActorEvent = 12,
		CustomIntProperty = 13,
		CustomFloatProperty = 14,
		CustomStringProperty = 15,
		CustomBooleanProperty = 16,
		ColliderRectangle = 17,
		ColliderTriangle = 18,
		ColliderCircle = 19,
		ColliderPolygon = 20,
		ColliderLine = 21,
		ActorNodeSolo = 23,
		NestedActorNode = 24,
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
		ActorSkin = 113
	};

	class ActorComponent
	{
	protected:
		ComponentType m_Type;
		std::string m_Name;
		union {
			unsigned short m_ParentIdx;
			ActorNode* m_Parent;
		};
		ActorArtboard* m_Artboard;
		std::vector<CustomIntProperty*> m_CustomIntProperties;
		std::vector<CustomFloatProperty*> m_CustomFloatProperties;
		std::vector<CustomStringProperty*> m_CustomStringProperties;
		std::vector<CustomBooleanProperty*> m_CustomBooleanProperties;
		std::vector<ActorComponent*> m_Dependents;

	public:
		// Used by the DAG in Actor.
		unsigned int m_GraphOrder;
		DirtyFlags m_DirtMask;

	private:
		void parent(ActorNode* value);

	protected:
		ActorComponent(ComponentType type);
		ActorComponent(ActorArtboard* actor, ComponentType type);
		virtual void onParentChanged(ActorNode* from, ActorNode* to) {}
		unsigned short parentIdx() const;

	public:
		virtual ~ActorComponent();
		ActorArtboard* artboard() const;
		const std::string& name() const;
		ComponentType type() const;

		ActorNode* parent() const;
		int componentIndex() const;
		virtual void resolveComponentIndices(ActorComponent** components, int numComponents);
		virtual void completeResolve();
		virtual ActorComponent* makeInstance(ActorArtboard* artboard) const = 0;
		void copy(const ActorComponent* node, ActorArtboard* artboard);
		virtual bool isNode() { return false; }

		static ActorComponent* read(ActorArtboard* artboard, BlockReader* reader, ActorComponent* component = NULL);

		void addCustomIntProperty(CustomIntProperty* property);
		void addCustomFloatProperty(CustomFloatProperty* property);
		void addCustomStringProperty(CustomStringProperty* property);
		void addCustomBooleanProperty(CustomBooleanProperty* property);

		CustomIntProperty* getCustomIntProperty(const std::string& name);
		CustomFloatProperty* getCustomFloatProperty(const std::string& name);
		CustomStringProperty* getCustomStringProperty(const std::string& name);
		CustomBooleanProperty* getCustomBooleanProperty(const std::string& name);
		std::vector<ActorComponent*>& dependents() { return m_Dependents; }

		virtual void onDirty(DirtyFlags dirt) {}
		virtual void update(DirtyFlags dirt) {}
	};
} // namespace flare
#endif