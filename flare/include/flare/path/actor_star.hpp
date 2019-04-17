#ifndef _FLARE_ACTORSTAR_HPP_
#define _FLARE_ACTORSTAR_HPP_

#include "flare/path/actor_procedural_path.hpp"
#include <vector>

namespace flare
{
	class ActorStar : public ActorProceduralPath
	{
		typedef ActorProceduralPath Base;

	private:
		int m_NumPoints;
		float m_InnerRadius;

	public:
		ActorStar();

		void copy(const ActorStar* star, ActorArtboard* artboard);
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
        void makePoints() override;
		static ActorStar* read(ActorArtboard* artboard, BlockReader* reader, ActorStar* component = nullptr);

		float innerRadius() const { return m_InnerRadius; }
		bool innerRadius(float value);
	};
} // namespace flare
#endif