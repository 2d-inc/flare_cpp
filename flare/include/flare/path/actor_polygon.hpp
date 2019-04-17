#ifndef _FLARE_ACTORPOLYGON_HPP_
#define _FLARE_ACTORPOLYGON_HPP_

#include "actor_procedural_path.hpp"
#include <vector>

namespace flare
{
	class ActorPolygon : public ActorProceduralPath
	{
		typedef ActorProceduralPath Base;

	private:
		int m_NumSides;

	public:
		ActorPolygon();

        int numSides() const { return m_NumSides; }
        void numSides(int value);

		void copy(const ActorPolygon* star, ActorArtboard* artboard);
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void makePoints() override;
		static ActorPolygon* read(ActorArtboard* artboard, BlockReader* reader, ActorPolygon* component);
	};
} // namespace flare
#endif