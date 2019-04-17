#include "flare/actor_image.hpp"
#include "flare/actor.hpp"
#include "flare/actor_bone_base.hpp"
#include "flare/block_reader.hpp"
#include <cstring>

using namespace flare;

ActorImage::ActorImage() :
    ActorDrawable(ComponentType::ActorImage),

    m_IsInstance(false),
    m_TextureIndex(-1),
    m_Vertices(nullptr),
    m_Triangles(nullptr),
    m_VertexCount(0),
    m_TriangleCount(0),
    m_AnimationDeformedVertices(nullptr),
    // m_IsVertexDeformDirty(false),
    m_SequenceFrames(nullptr),
    m_SequenceUVs(nullptr),
    m_SequenceFrame(0),
    m_SequenceFramesCount(0)
{
}

ActorImage::~ActorImage()
{
	if (!m_IsInstance)
	{
		delete[] m_Vertices;
		delete[] m_Triangles;
		if (m_SequenceFramesCount > 0)
		{
			delete[] m_SequenceUVs;
			delete[] m_SequenceFrames;
		}
	}
	delete[] m_AnimationDeformedVertices;
}

ActorComponent* ActorImage::makeInstance(ActorArtboard* artboard) const
{
	ActorImage* instanceNode = artboard->actor()->makeImageNode();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

bool ActorImage::doesAnimationVertexDeform() const { return m_AnimationDeformedVertices != nullptr; }

void ActorImage::doesAnimationVertexDeform(bool doesIt)
{
	if (doesIt)
	{
		delete[] m_AnimationDeformedVertices;
		m_AnimationDeformedVertices = new float[m_VertexCount * 2];

		// Copy the deform verts from the rig verts.
		int writeIdx = 0;
		int readIdx = 0;
		int readStride = vertexStride();
		for (int i = 0; i < m_VertexCount; i++)
		{
			m_AnimationDeformedVertices[writeIdx++] = m_Vertices[readIdx];
			m_AnimationDeformedVertices[writeIdx++] = m_Vertices[readIdx + 1];
			readIdx += readStride;
		}
	}
	else
	{
		delete[] m_AnimationDeformedVertices;
		m_AnimationDeformedVertices = nullptr;
	}
}

float* ActorImage::animationDeformedVertices() { return m_AnimationDeformedVertices; }

// bool ActorImage::isVertexDeformDirty() const { return m_IsVertexDeformDirty; }

// void ActorImage::isVertexDeformDirty(bool isIt) { m_IsVertexDeformDirty = isIt; }

void ActorImage::disposeGeometry()
{
	if (m_IsInstance)
	{
		return;
	}
	delete[] m_Vertices;
	m_Vertices = nullptr;
	delete[] m_Triangles;
	m_Triangles = nullptr;
}

void ActorImage::copy(const ActorImage* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	copySkinnable(node, artboard);

	m_IsInstance = true;
	m_TextureIndex = node->m_TextureIndex;
	m_VertexCount = node->m_VertexCount;
	m_TriangleCount = node->m_TriangleCount;
	m_Vertices = node->m_Vertices;
	m_Triangles = node->m_Triangles;
	// m_SequenceFrames = node->m_SequenceFrames;
	// m_SequenceFramesCount = node->m_SequenceFramesCount;
	if (node->m_AnimationDeformedVertices != nullptr)
	{
		int deformedVertexLength = m_VertexCount * 2;
		m_AnimationDeformedVertices = new float[deformedVertexLength];
		std::memmove(m_AnimationDeformedVertices, node->m_AnimationDeformedVertices,
		             deformedVertexLength * sizeof(float));
	}
}

ActorImage* ActorImage::readImageSequence(ActorArtboard* artboard, BlockReader* reader, ActorImage* node)
{
	ActorImage::read(artboard, reader, node);

	if (node->textureIndex() >= 0)
	{
		int frameAssetCount = (int)reader->readUint16();
		node->m_SequenceFramesCount = frameAssetCount;
		node->m_SequenceUVs = new float[node->vertexCount() * 2 * frameAssetCount];

		node->m_SequenceFrames = new SequenceFrame[frameAssetCount];
		// Init first frame
		node->m_SequenceFrames[0] = {node->textureIndex(), 0};
		int readIdx = 2;
		int writeIdx = 0;

		for (int i = 0; i < node->m_VertexCount; i++)
		{
			node->m_SequenceUVs[writeIdx++] = node->m_Vertices[readIdx];
			node->m_SequenceUVs[writeIdx++] = node->m_Vertices[readIdx + 1];
			readIdx += node->vertexStride();
		}

		int uvStride = node->vertexCount() * 2;
		int offset = uvStride;
		for (int i = 1; i < frameAssetCount; i++)
		{
			SequenceFrame& sf = node->m_SequenceFrames[i];
			sf = {reader->readUint8(), offset * 4};
			reader->readFloat32Array(node->m_SequenceUVs + offset, uvStride);
			offset += uvStride;
		}
	}
	return node;
}

ActorImage* ActorImage::read(ActorArtboard* artboard, BlockReader* reader, ActorImage* node)
{
	if (node == nullptr)
	{
		node = new ActorImage();
	}

	Base::read(artboard, reader, node);
	ActorSkinnable::read(artboard, reader, node);

	if (!node->isHidden())
	{
		node->m_TextureIndex = (int)reader->readUint8();
		unsigned int numVertices = reader->readUint32();
		int vertexStride = node->isConnectedToBones() ? 12 : 4;
		node->m_VertexCount = (int)numVertices;

		unsigned int vertexLength = numVertices * vertexStride;
		node->m_Vertices = new float[vertexLength];
		reader->readFloat32Array(node->m_Vertices, vertexLength);

		unsigned int numTris = reader->readUint32();
		node->m_TriangleCount = (int)numTris;

		unsigned int triLength = numTris * 3;
		node->m_Triangles = new unsigned short[triLength];
		reader->readUint16Array(node->m_Triangles, triLength);
	}
	return node;
}

void ActorImage::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);
	resolveSkinnable(components, numComponents);
}

int ActorImage::textureIndex() const { return m_TextureIndex; }

int ActorImage::vertexCount() const { return m_VertexCount; }

int ActorImage::triangleCount() const { return m_TriangleCount; }

float* ActorImage::vertices() const { return m_Vertices; }

unsigned short* ActorImage::triangles() const { return m_Triangles; }

int ActorImage::vertexStride() const { return isConnectedToBones() > 0 ? 12 : 4; }

BlendMode ActorImage::blendMode() const { return m_BlendMode; }

int ActorImage::sequenceFramesCount() const { return m_SequenceFramesCount; }

SequenceFrame* ActorImage::sequenceFrames() const { return m_SequenceFrames; }

float* ActorImage::sequenceUVs() const { return m_SequenceUVs; }

void ActorImage::sequenceFrame(int value) { m_SequenceFrame = value; }

int ActorImage::sequenceFrame() const { return m_SequenceFrame; }

// Only reason this is defined is so we can test loading. Concrete type must implement this.
void ActorImage::invalidateDrawable() {}

void ActorImage::initializeGraphics() {}