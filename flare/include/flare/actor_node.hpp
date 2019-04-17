#ifndef _FLARE_ACTORNODE_HPP_
#define _FLARE_ACTORNODE_HPP_

#include "actor_component.hpp"
#include "actor_constraint.hpp"
#include "flare_math/mat2d.hpp"
#include "flare_math/vec2d.hpp"
#include <functional>
#include <vector>

namespace flare
{
	class Actor;
	class BlockReader;
	union ActorNodeClip
	{
		unsigned short index;
		ActorNode* node;

		ActorNodeClip(unsigned short idx) : index(idx) {}
		ActorNodeClip(ActorNode* n) : node(n) {}
	};

	class ActorNode : public ActorComponent
	{
		typedef ActorComponent Base;

	protected:
		std::vector<ActorNode*> m_Children;
		std::vector<ActorConstraint*> m_Constraints;
		std::vector<ActorConstraint*> m_PeerConstraints;
		Mat2D m_Transform;
		Mat2D m_WorldTransform;
		Vec2D m_Translation;
		float m_Rotation;
		Vec2D m_Scale;
		float m_Opacity;
		float m_RenderOpacity;
		std::vector<ActorNodeClip> m_Clips;

	protected:
		bool m_OverrideWorldTransform;
		bool m_IsCollapsedVisibility;
		bool m_RenderCollapsed;

	protected:
		ActorNode(ComponentType type);
		virtual void onRenderOpacityChanged(){};

	private:
		ActorNode(ActorArtboard* artboard, ComponentType type);

	public:
		ActorNode(ActorArtboard* artboard);
		ActorNode();
		virtual ~ActorNode();
		const Mat2D& transform() const;
		const Mat2D& worldTransform() const;
		Mat2D& mutableTransform();
		Mat2D& mutableWorldTransform();
		void overrideWorldTransform(const Mat2D& transform);
		void clearWorldTransformOverride();

		const Vec2D& translation() const;
		void translation(const Vec2D& v);
		float x() const;
		void x(float v);
		float y() const;
		void y(float v);
		float scaleX() const;
		void scaleX(float v);
		float scaleY() const;
		void scaleY(float v);
		float rotation() const;
		void rotation(float v);
		float opacity() const;
		void opacity(float v);
		float renderOpacity() const;
		bool collapsedVisibility() const;
		void collapsedVisibility(bool v);
		bool renderCollapsed() const;
		void markTransformDirty();
		void worldTranslation(Vec2D& result);
		Vec2D worldTranslation();
		void updateTransform();
		virtual void updateWorldTransform();
		void addChild(ActorNode* node);
		void removeChild(ActorNode* node);
		void resolveComponentIndices(ActorComponent** components, int numComponents) override;
		ActorComponent* makeInstance(ActorArtboard* resetActor) const override;
		void copy(const ActorNode* node, ActorArtboard* resetActor);
		bool isNode() override { return true; }

		static ActorNode* read(ActorArtboard* artboard, BlockReader* reader, ActorNode* node = nullptr);

		bool addConstraint(ActorConstraint* constraint);
		bool addPeerConstraint(ActorConstraint* constraint);
		std::vector<ActorConstraint*> allConstraints() const;
		void update(DirtyFlags dirt) override;
		virtual void onChildrenChanged() {}
		virtual void onClipsChanged() {}

		const std::vector<ActorNode*>& children() const { return m_Children; }
		const std::vector<ActorNodeClip>& clips() const { return m_Clips; }

		bool eachChildRecursive(std::function<bool(ActorNode*)> cb)
		{
			for (auto child : m_Children)
			{
				if (!cb(child))
				{
					return false;
				}
				if (!child->eachChildRecursive(cb))
				{
					return false;
				}
			}

			return true;
		}

		void all(std::function<bool(ActorNode*)> cb)
		{
			if (!cb(this))
			{
				return;
			}
			eachChildRecursive(cb);
		}
	};
} // namespace flare
#endif