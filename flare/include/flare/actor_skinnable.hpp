#ifndef _FLARE_ACTORSKINNABLE_HPP_
#define _FLARE_ACTORSKINNABLE_HPP_

#include "actor_node.hpp"
#include "blend_mode.hpp"
#include <vector>

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;
	class ActorSkin;

	class ActorSkinnable
	{
		friend class ActorSkin;

		struct SkinnedBone
		{
			union {
				int boneIndex;
				ActorNode* node;
			};
			Mat2D bind;
			Mat2D inverseBind;

			SkinnedBone();
		};

	private:
		ActorSkin* m_Skin;
		std::vector<SkinnedBone> m_ConnectedBones;

	public:
		ActorSkinnable();
		ActorSkin* skin() const;
		bool isConnectedToBones() const { return !m_ConnectedBones.empty(); }
        static ActorSkinnable* read(ActorArtboard* artboard, BlockReader* reader, ActorSkinnable* skinnable);
        void resolveSkinnable(ActorComponent** components, int numComponents);
        void copySkinnable(const ActorSkinnable* skinnable, ActorArtboard* artboard);

		virtual void invalidateDrawable() = 0;
        virtual void overrideWorldTransform(const Mat2D& transform) = 0;
	};
} // namespace flare
#endif