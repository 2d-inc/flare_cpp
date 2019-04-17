#ifndef _FLARE_ACTOR_HPP_
#define _FLARE_ACTOR_HPP_

#include "actor_image.hpp"
#include "actor_node.hpp"
#include "actor_static_mesh.hpp"
#include "animation/actor_animation.hpp"
#include "animation/actor_animation_instance.hpp"
#include "actor_artboard.hpp"
#include "block_reader.hpp"
#include "nested_actor_asset.hpp"
#include "nested_actor_node.hpp"
#include <vector>
#include "block_types.hpp"

namespace flare
{
	class ActorShape;
	class ActorColorFill;
	class ActorColorStroke;
	class ActorGradientFill;
	class ActorGradientStroke;
	class ActorRadialGradientFill;
	class ActorRadialGradientStroke;
	class ActorPath;
	class ActorRectangle;
	class ActorTriangle;
	class ActorPolygon;
	class ActorStar;
	class ActorEllipse;
	class ActorDrawableProxy;

	class Actor
	{
		friend class ActorArtboard;
		friend class ActorAnimationInstance;
		friend class NestedActorNode;
		friend class ActorImage;
		friend class ActorStaticMesh;

	public:
		Actor();
		virtual ~Actor();

	private:
  
		void readArtboardsBlock(BlockReader* block);
		

	protected:
		int m_MaxTextureIndex;
		unsigned int m_Version;
		unsigned int m_ArtboardCount;
		ActorArtboard** m_Artboards;
		std::string m_BaseFilename;
		virtual void dispose();

	public:
		virtual ActorArtboard* makeArtboard();
		virtual ActorNode* makeNode();
		virtual ActorImage* makeImageNode();
		virtual ActorStaticMesh* makeStaticMeshNode();
		virtual NestedActorNode* makeNestedActorNode();
		virtual NestedActorAsset* makeNestedActorAsset();
		virtual ActorShape* makeShape();
		virtual ActorColorFill* makeColorFill();
		virtual ActorColorStroke* makeColorStroke();
		virtual ActorGradientFill* makeGradientFill();
		virtual ActorGradientStroke* makeGradientStroke();
		virtual ActorRadialGradientFill* makeRadialGradientFill();
		virtual ActorRadialGradientStroke* makeRadialGradientStroke();
		virtual ActorPath* makePath();
		virtual ActorRectangle* makeRectangle();
		virtual ActorTriangle* makeTriangle();
		virtual ActorPolygon* makePolygon();
		virtual ActorStar* makeStar();
		virtual ActorEllipse* makeEllipse();
		virtual ActorDrawableProxy* makeDrawableProxy(ActorDrawable* drawable);

		void load(unsigned char* bytes, unsigned int length);
		void load(const std::string& filename);
		void setFilename(const std::string& filename);

		void copy(const Actor& actor);
		const int textureCount() const;
		const std::string& baseFilename() const;

		virtual Actor* makeInstance() const;
		template <typename T> T* artboard() const { return m_ArtboardCount > 0 ? dynamic_cast<T*>(m_Artboards[0]) : nullptr; }

		template <typename T> T* instance() const { return dynamic_cast<T*>(makeInstance()); }
	};
} // namespace flare
#endif