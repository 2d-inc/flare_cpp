#ifndef _FLARE_ACTORSTATICMESH_HPP_
#define _FLARE_ACTORSTATICMESH_HPP_

#include "actor_drawable.hpp"
#include "blend_mode.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;

	class ActorStaticMesh : public ActorDrawable
	{
		typedef ActorDrawable Base;

	private:
		bool m_IsInstance;
		BlendMode m_BlendMode;

	public:
		struct Surface
		{
			int textureIndex;
			unsigned int vertexCount;
			unsigned int triangleCount;
			float* vertices;
			unsigned short* triangles;

			Surface();
			virtual ~Surface();
			virtual void disposeGeometry();
		};

	protected:
		int m_NumSurfaces;
		Surface** m_Surfaces;

		virtual Surface* makeSurface();

	public:
		ActorStaticMesh();
		~ActorStaticMesh();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorStaticMesh* node, ActorArtboard* artboard);
		void resolveComponentIndices(ActorComponent** nodes, int numComponents) override;
		void initializeGraphics() override;
		virtual void disposeGeometry();
		BlendMode blendMode() const;
		int numSurfaces() const;
		Surface* surface(int index);

		static ActorStaticMesh* read(ActorArtboard* artboard, BlockReader* reader, ActorStaticMesh* node = NULL);
	};
} //namespace flare
#endif