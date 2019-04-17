#include "flare/nested_actor_asset.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

NestedActorAsset::NestedActorAsset() {}

NestedActorAsset::~NestedActorAsset() {}

const std::string& NestedActorAsset::name() const { return m_Name; }

const std::string& NestedActorAsset::id() const { return m_Id; }

void NestedActorAsset::read(BlockReader* reader)
{
	m_Name = reader->readString();
	m_Id = reader->readString();
}

Actor* NestedActorAsset::actor() { return nullptr; }