#include "flare/animation/keyframes/keyframe_image_vertices.hpp"
#include "flare/actor_image.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

KeyFrameImageVertices::KeyFrameImageVertices() :
	m_Vertices(nullptr),
	m_VerticesCount(0)
{

}

KeyFrameImageVertices::~KeyFrameImageVertices()
{
	delete [] m_Vertices;
}

bool KeyFrameImageVertices::read(BlockReader* reader, ActorComponent* component)
{
	if(!Base::read(reader, component))
	{
		return false;
	}

	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return false;
	}
	m_VerticesCount = imageNode->vertexCount() * 2;
	m_Vertices = new float[m_VerticesCount];
	reader->readFloat32Array(m_Vertices, m_VerticesCount);
	
	imageNode->doesAnimationVertexDeform(true);
	return true;
}

void KeyFrameImageVertices::apply(ActorComponent* component, float mix)
{
	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return;
	}
	int l = m_VerticesCount;
	float* wr = imageNode->animationDeformedVertices();
	if(mix == 1.0f)
	{
		for(int i = 0; i < l; i++)
		{
			wr[i] = m_Vertices[i];
		}
	}
	else
	{
		float mixi = 1.0f - mix;
		for(int i = 0; i < l; i++)
		{
			wr[i] = wr[i] * mixi + m_Vertices[i] * mix;
		}
	}

	imageNode->invalidateDrawable();
}

void KeyFrameImageVertices::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	ActorImage* imageNode = reinterpret_cast<ActorImage*>(component);
	if(imageNode == nullptr)
	{
		return;
	}
	float* wr = imageNode->animationDeformedVertices();
	float* to = reinterpret_cast<KeyFrameImageVertices*>(toFrame)->m_Vertices;
	int l = m_VerticesCount;

	float f = (time - m_Time)/(toFrame->time()-m_Time);
	float fi = 1.0f - f;
	if(mix == 1.0f)
	{
		for(int i = 0; i < l; i++)
		{
			wr[i] = m_Vertices[i] * fi + to[i] * f;
		}
	}
	else
	{
		float mixi = 1.0f - mix;
		for(int i = 0; i < l; i++)
		{
			float v = m_Vertices[i] * fi + to[i] * f;

			wr[i] = wr[i] * mixi + v * mix;
		}
	}

	imageNode->invalidateDrawable();
}