#include "flare/actor_static_mesh.hpp"
#include "flare/actor.hpp"
#include "flare/actor_bone.hpp"
#include "flare/block_reader.hpp"
#include <cstring>

using namespace flare;

ActorStaticMesh::ActorStaticMesh() :
    ActorDrawable(ComponentType::ActorStaticMesh),

    m_IsInstance(false),
    m_BlendMode(BlendMode::SrcOver),
    m_NumSurfaces(0),
    m_Surfaces(nullptr)
{
}

ActorStaticMesh::Surface::Surface() :
    textureIndex(0),
    vertexCount(0),
    triangleCount(0),
    vertices(nullptr),
    triangles(nullptr)
{
}

ActorStaticMesh::Surface::~Surface() { disposeGeometry(); }

void ActorStaticMesh::Surface::disposeGeometry()
{
	delete[] vertices;
	delete[] triangles;

	vertices = nullptr;
	triangles = nullptr;
}

ActorStaticMesh::~ActorStaticMesh()
{
	if (m_IsInstance)
	{
		return;
	}

	for (int i = 0; i < m_NumSurfaces; i++)
	{
		delete m_Surfaces[i];
	}
	delete[] m_Surfaces;
}

ActorComponent* ActorStaticMesh::makeInstance(ActorArtboard* artboard) const
{
	ActorStaticMesh* instanceNode = new ActorStaticMesh();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

void ActorStaticMesh::disposeGeometry()
{
	if (m_IsInstance)
	{
		return;
	}

	for (int i = 0; i < m_NumSurfaces; i++)
	{
		m_Surfaces[i]->disposeGeometry();
	}
	//	delete [] m_Vertices;
	//	m_Vertices = nullptr;
	//	delete [] m_Triangles;
	//	m_Triangles = nullptr;
}

void ActorStaticMesh::copy(const ActorStaticMesh* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	m_IsInstance = true;
	m_BlendMode = node->m_BlendMode;
	m_NumSurfaces = node->m_NumSurfaces;
	m_Surfaces = node->m_Surfaces;
}

ActorStaticMesh::Surface* ActorStaticMesh::makeSurface() { return new Surface(); }

ActorStaticMesh* ActorStaticMesh::read(ActorArtboard* artboard, BlockReader* reader, ActorStaticMesh* node)
{
	if (node == nullptr)
	{
		node = new ActorStaticMesh();
	}

	Base::read(artboard, reader, node);

	bool isVisible = reader->readUint8() != 0;
	if (isVisible)
	{
		node->m_BlendMode = (BlendMode)reader->readUint8();
		node->m_DrawOrder = (int)reader->readUint16();

		int surfaceCount = (int)reader->readUint16();
		node->m_NumSurfaces = surfaceCount;
		node->m_Surfaces = new Surface*[surfaceCount];

		for (int i = 0; i < surfaceCount; i++)
		{
			auto surface = node->makeSurface();
			node->m_Surfaces[i] = surface;
			surface->textureIndex = (int)reader->readUint8();

			surface->vertexCount = reader->readUint32();
			const int vertexStride = 4;
			unsigned int vertexLength = surface->vertexCount * vertexStride;
			surface->vertices = new float[vertexLength];
			reader->readFloat32Array(surface->vertices, vertexLength);

			surface->triangleCount = reader->readUint32();
			unsigned int triLength = surface->triangleCount * 3;
			surface->triangles = new unsigned short[triLength];
			reader->readUint16Array(surface->triangles, triLength);
		}
	}
	return node;
}

void ActorStaticMesh::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
}

BlendMode ActorStaticMesh::blendMode() const { return m_BlendMode; }

int ActorStaticMesh::numSurfaces() const { return m_NumSurfaces; }

ActorStaticMesh::Surface* ActorStaticMesh::surface(int index) { return m_Surfaces[index]; }

void ActorStaticMesh::initializeGraphics() {}