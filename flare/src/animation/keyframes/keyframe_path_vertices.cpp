#include "flare/animation/keyframes/keyframe_path_vertices.hpp"
#include "flare/path/actor_path.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

KeyFramePathVertices::KeyFramePathVertices() :
	m_Vertices(nullptr),
	m_VerticesLength(0)
{

}

KeyFramePathVertices::~KeyFramePathVertices()
{
	delete [] m_Vertices;
}

bool KeyFramePathVertices::read(BlockReader* reader, ActorComponent* component)
{
	if(component->type() != ComponentType::ActorPath || !Base::read(reader, component))
	{
		return false;
	}

	ActorPath* path = reinterpret_cast<ActorPath*>(component);
	if(path == nullptr)
	{
		return false;
	}
    auto points = path->points();

    int length = 0;
    for(auto point : points)
    {
        length += 2 + (point->type() == PointType::Straight ? 1 : 4);
    }
    m_VerticesLength = length;
    m_Vertices = new float[length];
	reader->readFloat32Array(m_Vertices, m_VerticesLength);
	path->makeVertexDeform();
	return true;
}

void KeyFramePathVertices::apply(ActorComponent* component, float mix)
{
	ActorPath* path = reinterpret_cast<ActorPath*>(component);
	if(path == nullptr)
	{
		return;
	}
	int l = m_VerticesLength;
	float* wr = path->vertexDeform();
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

	path->invalidateDrawable();
}

void KeyFramePathVertices::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	ActorPath* path = reinterpret_cast<ActorPath*>(component);
	if(path == nullptr)
	{
		return;
	}
	float* wr = path->vertexDeform();
	float* to = reinterpret_cast<KeyFramePathVertices*>(toFrame)->m_Vertices;
	int l = m_VerticesLength;

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

	path->invalidateDrawable();
}