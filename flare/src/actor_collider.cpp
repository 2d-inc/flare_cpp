#include "flare/actor_collider.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

ActorCollider::ActorCollider(ComponentType type) : ActorNode(type), m_IsCollisionEnabled(true) {}

void ActorCollider::copy(const ActorCollider* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);

	m_IsCollisionEnabled = node->m_IsCollisionEnabled;
}

ActorCollider* ActorCollider::read(ActorArtboard* artboard, BlockReader* reader, ActorCollider* collider)
{
	Base::read(artboard, reader, collider);
	collider->m_IsCollisionEnabled = reader->readUint8() == 1;
	return collider;
}

bool ActorCollider::isCollisionEnabled() const { return m_IsCollisionEnabled; }

void ActorCollider::isCollisionEnabled(bool isIt)
{
	if (m_IsCollisionEnabled != isIt)
	{
		m_IsCollisionEnabled = isIt;
	}
}
// Rectangle

ActorColliderRectangle::ActorColliderRectangle() :
    ActorCollider(ComponentType::ColliderRectangle),
    m_Width(0.0f),
    m_Height(0.0f)
{
}

ActorComponent* ActorColliderRectangle::makeInstance(ActorArtboard* artboard) const
{
	ActorColliderRectangle* instanceProp = new ActorColliderRectangle();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void ActorColliderRectangle::copy(const ActorColliderRectangle* collider, ActorArtboard* artboard)
{
	Base::copy(collider, artboard);
	m_Width = collider->m_Width;
	m_Height = collider->m_Height;
}

ActorColliderRectangle* ActorColliderRectangle::read(ActorArtboard* artboard, BlockReader* reader,
                                                     ActorColliderRectangle* collider)
{
	if (collider == nullptr)
	{
		collider = new ActorColliderRectangle();
	}
	Base::read(artboard, reader, collider);
	collider->m_Width = reader->readFloat32();
	collider->m_Height = reader->readFloat32();
	return collider;
}

float ActorColliderRectangle::width() const { return m_Width; }

float ActorColliderRectangle::height() const { return m_Height; }

// Triangle

ActorColliderTriangle::ActorColliderTriangle() :
    ActorCollider(ComponentType::ColliderTriangle),
    m_Width(0.0f),
    m_Height(0.0f)
{
}

ActorComponent* ActorColliderTriangle::makeInstance(ActorArtboard* artboard) const 
{
	ActorColliderTriangle* instanceProp = new ActorColliderTriangle();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void ActorColliderTriangle::copy(const ActorColliderTriangle* collider, ActorArtboard* artboard)
{
	Base::copy(collider, artboard);
	m_Width = collider->m_Width;
	m_Height = collider->m_Height;
}

ActorColliderTriangle* ActorColliderTriangle::read(ActorArtboard* artboard, BlockReader* reader, ActorColliderTriangle* collider)
{
	if (collider == nullptr)
	{
		collider = new ActorColliderTriangle();
	}
	Base::read(artboard, reader, collider);
	collider->m_Width = reader->readFloat32();
	collider->m_Height = reader->readFloat32();
	return collider;
}

float ActorColliderTriangle::width() const { return m_Width; }

float ActorColliderTriangle::height() const { return m_Height; }

// Circle

ActorColliderCircle::ActorColliderCircle() : ActorCollider(ComponentType::ColliderCircle), m_Radius(0.0f) {}

ActorComponent* ActorColliderCircle::makeInstance(ActorArtboard* artboard) const
{
	ActorColliderCircle* instanceProp = new ActorColliderCircle();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void ActorColliderCircle::copy(const ActorColliderCircle* collider, ActorArtboard* artboard)
{
	Base::copy(collider, artboard);
	m_Radius = collider->m_Radius;
}

ActorColliderCircle* ActorColliderCircle::read(ActorArtboard* artboard, BlockReader* reader, ActorColliderCircle* collider)
{
	if (collider == nullptr)
	{
		collider = new ActorColliderCircle();
	}
	Base::read(artboard, reader, collider);
	collider->m_Radius = reader->readFloat32();
	return collider;
}

float ActorColliderCircle::radius() const { return m_Radius; }

// Polygon

ActorColliderPolygon::ActorColliderPolygon() :
    ActorCollider(ComponentType::ColliderPolygon),
    m_IsInstance(false),
    m_ContourVertexCount(0),
    m_ContourVertices(nullptr)
{
}

ActorColliderPolygon::~ActorColliderPolygon()
{
	if (!m_IsInstance)
	{
		delete[] m_ContourVertices;
	}
}

ActorComponent* ActorColliderPolygon::makeInstance(ActorArtboard* artboard) const 
{
	ActorColliderPolygon* instanceProp = new ActorColliderPolygon();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void ActorColliderPolygon::copy(const ActorColliderPolygon* collider, ActorArtboard* artboard)
{
	Base::copy(collider, artboard);
	m_IsInstance = true;
	m_ContourVertexCount = collider->m_ContourVertexCount;
	m_ContourVertices = collider->m_ContourVertices;
}

ActorColliderPolygon* ActorColliderPolygon::read(ActorArtboard* artboard, BlockReader* reader, ActorColliderPolygon* collider)
{
	if (collider == nullptr)
	{
		collider = new ActorColliderPolygon();
	}
	Base::read(artboard, reader, collider);

	collider->m_ContourVertexCount = reader->readUint32();
	collider->m_ContourVertices = new float[collider->m_ContourVertexCount * 2];
	reader->readFloat32Array(collider->m_ContourVertices, collider->m_ContourVertexCount * 2);

	return collider;
}

float* ActorColliderPolygon::contourVertices() const { return m_ContourVertices; }

unsigned int ActorColliderPolygon::contourVertexCount() const { return m_ContourVertexCount; }

// Line

ActorColliderLine::ActorColliderLine() :
    ActorCollider(ComponentType::ColliderLine),
    m_IsInstance(false),
    m_VertexCount(0),
    m_Vertices(nullptr)
{
}

ActorColliderLine::~ActorColliderLine()
{
	if (!m_IsInstance)
	{
		delete[] m_Vertices;
	}
}

ActorComponent* ActorColliderLine::makeInstance(ActorArtboard* artboard) const 
{
	ActorColliderLine* instanceProp = new ActorColliderLine();
	instanceProp->copy(this, artboard);
	return instanceProp;
}

void ActorColliderLine::copy(const ActorColliderLine* collider, ActorArtboard* artboard)
{
	Base::copy(collider, artboard);
	m_IsInstance = true;
	m_VertexCount = collider->m_VertexCount;
	m_Vertices = collider->m_Vertices;
}

ActorColliderLine* ActorColliderLine::read(ActorArtboard* artboard, BlockReader* reader, ActorColliderLine* collider)
{
	if (collider == nullptr)
	{
		collider = new ActorColliderLine();
	}
	Base::read(artboard, reader, collider);

	collider->m_VertexCount = reader->readUint32();
	collider->m_Vertices = new float[collider->m_VertexCount * 2];
	reader->readFloat32Array(collider->m_Vertices, collider->m_VertexCount * 2);

	return collider;
}

float* ActorColliderLine::vertices() const { return m_Vertices; }

unsigned int ActorColliderLine::vertexCount() const { return m_VertexCount; }