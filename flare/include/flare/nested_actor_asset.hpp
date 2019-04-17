#ifndef _FLARE_NESTEDACTORASSET_HPP_
#define _FLARE_NESTEDACTORASSET_HPP_

#include <functional>
#include <string>
#include <vector>

namespace flare
{
	class BlockReader;
	class Actor;

	class NestedActorAsset
	{
	protected:
		std::string m_Name;
		std::string m_Id;

	public:
		NestedActorAsset();
		virtual ~NestedActorAsset();

		const std::string& name() const;
		const std::string& id() const;

		virtual Actor* actor();
		void read(BlockReader* reader);
	};
} //namespace flare
#endif