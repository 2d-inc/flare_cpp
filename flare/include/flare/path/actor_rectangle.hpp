#ifndef _FLARE_ACTORRECTANGLE_HPP_
#define _FLARE_ACTORRECTANGLE_HPP_

#include "actor_procedural_path.hpp"
#include <vector>

namespace flare
{
	class ActorRectangle : public ActorProceduralPath
	{
		typedef ActorProceduralPath Base;
    private:
        float m_Radius;

	public:
		ActorRectangle();

        void copy(const ActorRectangle* rectangle, ActorArtboard* artboard);
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void makePoints() override;
        static ActorRectangle* read(ActorArtboard* artboard, BlockReader* reader, ActorRectangle* component = NULL);

		float radius() const { return m_Radius; }
		bool radius(float value);
	};
} // namespace flare
#endif