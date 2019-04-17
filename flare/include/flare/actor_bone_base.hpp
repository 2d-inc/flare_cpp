#ifndef _FLARE_ACTORBONEBASE_HPP_
#define _FLARE_ACTORBONEBASE_HPP_

#include "flare/actor_node.hpp"
#include "flare_math/vec2d.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorBoneBase : public ActorNode
	{
		typedef ActorNode Base;

	protected:
		float m_Length;
		bool m_IsConnectedToImage;

	public:
		ActorBoneBase(ComponentType type);
		float length() const;
		void length(float l);
		bool isConnectedToImage() const;
		void isConnectedToImage(bool isIt);
		void tipWorldTranslation(Vec2D& result);
		Vec2D tipWorldTranslation();
		void copy(const ActorBoneBase* node, ActorArtboard* artboard);
		static ActorBoneBase* read(ActorArtboard* artboard, BlockReader* reader, ActorBoneBase* node = NULL);
	};
} // namespace flare
#endif