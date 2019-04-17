#ifndef _FLARE_DEPENDENCYSORTER_HPP_
#define _FLARE_DEPENDENCYSORTER_HPP_

#include "actor_component.hpp"
#include <unordered_set>
#include <vector>

namespace flare
{
	class DependencySorter
	{
	private:
		std::unordered_set<ActorComponent*> m_Perm;
		std::unordered_set<ActorComponent*> m_Temp;

	public:
		void sort(ActorComponent* root, std::vector<ActorComponent*>& order);
		bool visit(ActorComponent* component, std::vector<ActorComponent*>& order);
	};
} // namespace flare

#endif