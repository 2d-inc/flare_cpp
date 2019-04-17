#ifndef _FLARE_ACTORSKIN_HPP_
#define _FLARE_ACTORSKIN_HPP_

#include "actor_component.hpp"
#include "blend_mode.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorSkinnable;

	class ActorSkin : public ActorComponent
	{
		typedef ActorComponent Base;

	private:
		float* m_BoneMatrices;
        int m_BoneMatricesLength;
        ActorSkinnable* m_SkinnableParent;

	public:
		ActorSkin();
		~ActorSkin();
		const float* boneMatrices() const;
		void update(DirtyFlags dirt) override;
		void completeResolve() override;
        ActorComponent* makeInstance(ActorArtboard* artboard) const override;

	protected:
		void onParentChanged(ActorNode* from, ActorNode* to) override;
	};
} // namespace flare
#endif