#ifndef _FLARE_NESTEDACTORNODE_HPP_
#define _FLARE_NESTEDACTORNODE_HPP_

#include "actor_drawable.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class NestedActorAsset;

	class NestedActorNode : public ActorDrawable
	{
		typedef ActorDrawable Base;

	private:
		NestedActorAsset* m_Asset;
		ActorArtboard* m_ArtboardInstance;
		ActorArtboard* m_TransformArtboardInstance;

	protected:
		virtual void setArtboardInstance(ActorArtboard* instance);

	public:
		NestedActorNode();
		virtual ~NestedActorNode();

		ActorComponent* makeInstance(ActorArtboard* resetActor) const override;
		void updateWorldTransform() override;
		void copy(const NestedActorNode* node, ActorArtboard* resetActor);
		void advance(float elapsedSeconds);
		void transformNested(bool transformIt);
		void initializeGraphics() override;
		
		static NestedActorNode* read(ActorArtboard* actor, BlockReader* reader, NestedActorNode* node = NULL);
	};
} //namespace flare
#endif