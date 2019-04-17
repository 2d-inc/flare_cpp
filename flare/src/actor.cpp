#include "flare/actor.hpp"
#include "flare/actor_bone.hpp"
#include "flare/actor_collider.hpp"
#include "flare/actor_distance_constraint.hpp"
#include "flare/actor_drawable_proxy.hpp"
#include "flare/actor_event.hpp"
#include "flare/actor_ik_constraint.hpp"
#include "flare/actor_jelly_bone.hpp"
#include "flare/actor_node_solo.hpp"
#include "flare/actor_root_bone.hpp"
#include "flare/actor_rotation_constraint.hpp"
#include "flare/actor_scale_constraint.hpp"
#include "flare/actor_shape.hpp"
#include "flare/actor_transform_constraint.hpp"
#include "flare/actor_translation_constraint.hpp"
#include "flare/binary_reader.hpp"
#include "flare/block_reader.hpp"
#include "flare/custom_property.hpp"
#include "flare/dependency_sorter.hpp"
#include "flare/exceptions/missing_file_exception.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include "flare/jelly_component.hpp"
#include "flare/paint/actor_color_fill.hpp"
#include "flare/paint/actor_color_stroke.hpp"
#include "flare/paint/actor_gradient_fill.hpp"
#include "flare/paint/actor_gradient_stroke.hpp"
#include "flare/paint/actor_radial_gradient_fill.hpp"
#include "flare/paint/actor_radial_gradient_stroke.hpp"
#include "flare/path/actor_ellipse.hpp"
#include "flare/path/actor_path.hpp"
#include "flare/path/actor_polygon.hpp"
#include "flare/path/actor_rectangle.hpp"
#include "flare/path/actor_star.hpp"
#include "flare/path/actor_triangle.hpp"
#include <algorithm>
#include <assert.h>
#include <stdio.h>

using namespace flare;

Actor::Actor() : m_MaxTextureIndex(-1), m_Version(0), m_ArtboardCount(0), m_Artboards(nullptr) {}

Actor::~Actor() { dispose(); }

void Actor::dispose()
{
	for (unsigned int i = 0; i < m_ArtboardCount; i++)
	{
		delete m_Artboards[i];
	}
	delete[] m_Artboards;
	m_Artboards = nullptr;
	m_MaxTextureIndex = -1;
}

void Actor::load(unsigned char* bytes, unsigned int length)
{
	dispose();

	BlockReader reader(bytes, length);

	unsigned char F = reader.readUint8();
	unsigned char L = reader.readUint8();
	unsigned char A = reader.readUint8();
	unsigned char R = reader.readUint8();
	unsigned char E = reader.readUint8();
	unsigned int version = reader.readUint32();

	// Make sure it's a nima file.
	if (F != 70 || L != 76 || A != 65 || R != 82 || E != 69)
	{
		throw UnsupportedVersionException("Unsupported file version", 0, 18);
	}
	// And of supported version...
	if (version < 18)
	{
		throw UnsupportedVersionException("Unsupported file version", version, 18);
	}

	BlockReader* block = nullptr;
	while ((block = reader.readNextBlock()) != nullptr)
	{
		switch (block->blockType<BlockType>())
		{
			case BlockType::Artboards:
				readArtboardsBlock(block);
				break;
			case BlockType::Atlases:
				// TODO: Load atlases
				break;
			default:
				break;
		}
		block->close();
	}
}

void Actor::readArtboardsBlock(BlockReader* block)
{
	m_ArtboardCount = (unsigned int)block->readUint16();
	m_Artboards = new ActorArtboard*[m_ArtboardCount];
	for (int artboardIndex = 0; artboardIndex < m_ArtboardCount; artboardIndex++)
	{
		BlockReader* artboardBlock = block->readNextBlock();
		if (artboardBlock == nullptr)
		{
			m_Artboards[artboardIndex] = nullptr;
		}
		else
		{
			switch (artboardBlock->blockType<BlockType>())
			{
				case BlockType::ActorArtboard:
				{
					ActorArtboard* artboard = makeArtboard();
					artboard->read(artboardBlock);
					m_Artboards[artboardIndex] = artboard;
					break;
				}
				default:
					break;
			}
		}
		artboardBlock->close();
	}
}

void Actor::setFilename(const std::string& filename)
{
	size_t index = filename.rfind('.');
	if (index == std::string::npos)
	{
		m_BaseFilename = filename;
	}
	else
	{
		m_BaseFilename = std::string(filename, 0, index);
	}
}

void Actor::load(const std::string& filename)
{
	setFilename(filename);

	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr)
	{
		throw MissingFileException("flare file is missing", filename);
	}
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* bytes = new unsigned char[length];
	fread(bytes, length, 1, fp);
	fclose(fp);

	try
	{
		load(bytes, (unsigned int)length);
		delete[] bytes;
	}
	catch (OverflowException ex)
	{
		delete[] bytes;
		throw ex;
	}
}

ActorArtboard* Actor::makeArtboard() { return new ActorArtboard(this); }
ActorNode* Actor::makeNode() { return new ActorNode(); }
ActorImage* Actor::makeImageNode() { return new ActorImage(); }

ActorStaticMesh* Actor::makeStaticMeshNode() { return new ActorStaticMesh(); }

NestedActorNode* Actor::makeNestedActorNode() { return new NestedActorNode(); }

NestedActorAsset* Actor::makeNestedActorAsset() { return new NestedActorAsset(); }
ActorShape* Actor::makeShape() { return new ActorShape(); }
ActorColorFill* Actor::makeColorFill() { return new ActorColorFill(); }
ActorColorStroke* Actor::makeColorStroke() { return new ActorColorStroke(); }
ActorGradientFill* Actor::makeGradientFill() { return new ActorGradientFill(); }
ActorGradientStroke* Actor::makeGradientStroke() { return new ActorGradientStroke(); }
ActorRadialGradientFill* Actor::makeRadialGradientFill() { return new ActorRadialGradientFill(); }
ActorRadialGradientStroke* Actor::makeRadialGradientStroke() { return new ActorRadialGradientStroke(); }
ActorPath* Actor::makePath() { return new ActorPath(); }
ActorDrawableProxy* Actor::makeDrawableProxy(ActorDrawable* drawable) { return new ActorDrawableProxy(drawable); }

ActorRectangle* Actor::makeRectangle() { return new ActorRectangle(); }

ActorTriangle* Actor::makeTriangle() { return new ActorTriangle(); }

ActorPolygon* Actor::makePolygon() { return new ActorPolygon(); }

ActorStar* Actor::makeStar() { return new ActorStar(); }

ActorEllipse* Actor::makeEllipse() { return new ActorEllipse(); }

void Actor::copy(const Actor& actor)
{
	m_MaxTextureIndex = actor.m_MaxTextureIndex;
	m_ArtboardCount = actor.m_ArtboardCount;
	m_Artboards = new ActorArtboard*[m_ArtboardCount];

	for (int i = 0; i < m_ArtboardCount; i++)
	{
		ActorArtboard* copyFrom = actor.m_Artboards[i];
		if (copyFrom != nullptr)
		{
			m_Artboards[i] = actor.m_Artboards[i]->makeInstance(this);
		}
		else
		{
			m_Artboards[i] = nullptr;
		}
	}
}

const int Actor::textureCount() const { return m_MaxTextureIndex + 1; }

const std::string& Actor::baseFilename() const { return m_BaseFilename; }

Actor* Actor::makeInstance() const
{
	assert(false);
	return nullptr;
}