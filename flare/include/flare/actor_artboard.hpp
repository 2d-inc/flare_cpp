#ifndef _FLARE_ACTOR_ARTBOARD_HPP_
#define _FLARE_ACTOR_ARTBOARD_HPP_

#include "actor_flags.hpp"
#include "actor_image.hpp"
#include "actor_node.hpp"
#include "actor_static_mesh.hpp"
#include "animation/actor_animation.hpp"
#include "animation/actor_animation_instance.hpp"
#include "block_reader.hpp"
#include "dirty_flags.hpp"
#include "flare_math/aabb.hpp"
#include "flare_math/color.hpp"
#include "flare_math/vec2d.hpp"
#include "nested_actor_asset.hpp"
#include "nested_actor_node.hpp"
#include <vector>

namespace flare
{
	class ActorDrawableProxy;

	class ActorArtboard
	{
		friend class ActorAnimationInstance;
		friend class NestedActorNode;
		friend class ActorImage;
		friend class ActorStaticMesh;
		friend class Actor;

	public:
		ActorArtboard(Actor* actor);
		virtual ~ActorArtboard();

	private:
		Actor* m_Actor;
		std::string m_Name;
		Vec2D m_Translation;
		Vec2D m_Origin;
		float m_Width;
		float m_Height;
		bool m_ClipContents;
		Color m_Color;
		ActorFlags m_Flags;
		unsigned int m_ComponentCount;
		unsigned int m_NodeCount;
		std::vector<ActorComponent*> m_DependencyOrder;
		unsigned int m_DirtDepth;
		ActorComponent** m_Components;
		ActorNode** m_Nodes;
		ActorNode* m_Root;
		void* m_EventCallbackUserData;
		ActorAnimationEvent::Callback m_EventCallback;
		void read(BlockReader* block);
		void readComponentsBlock(BlockReader* block);
		void readAnimationsBlock(BlockReader* block);
		void readNestedActorAssetsBlock(BlockReader* block);
		void sortDependencies();

	protected:
		unsigned int m_ImageNodeCount;
		unsigned int m_DrawableCount;
		unsigned int m_AnimationsCount;
		unsigned int m_NestedActorAssetCount;
		unsigned int m_NestedActorNodeCount;

		ActorImage** m_ImageNodes;
		ActorDrawableProxy** m_Drawables;
		ActorAnimation* m_Animations;
		NestedActorAsset** m_NestedActorAssets;
		NestedActorNode** m_NestedActorNodes;

		virtual void dispose();

	public:
		bool addDependency(ActorComponent* a, ActorComponent* b);

		unsigned int componentCount() const;
		ActorComponent* component(unsigned int index) const;
		ActorComponent* component(unsigned short index) const;
		ActorComponent* component(const std::string& name) const;
		int componentIndex(const ActorComponent* component) const;

		const int nestedActorCount() const;
		NestedActorAsset* nestedActorAsset(unsigned int index) const;

		void eventCallback(ActorAnimationEvent::Callback callback, void* userdata = nullptr);

		template <typename T> T component(const std::string& name) const { return dynamic_cast<T>(component(name)); }

		Actor* actor() const;
		ActorNode* root() const;
		ActorAnimation* animation(const std::string& name) const;
		ActorAnimationInstance* animationInstance(const std::string& name);

		void copy(const ActorArtboard& artboard);
		virtual void update();
		virtual void advance(float elapsedSeconds);
		bool addDirt(ActorComponent* component, DirtyFlags value, bool recurse = true);
		void markDrawOrderDirty();

		AABB aabb() const;

		virtual ActorArtboard* makeInstance(Actor* resetActor) const;
	};
} // namespace flare
#endif