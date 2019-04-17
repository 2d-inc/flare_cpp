#ifndef _FLARE_ACTORCOLLIDER_HPP_
#define _FLARE_ACTORCOLLIDER_HPP_

#include "actor_node.hpp"
#include <string>

namespace flare
{
	class ActorCollider : public ActorNode
	{
		typedef ActorNode Base;

	private:
		bool m_IsCollisionEnabled;

	public:
		ActorCollider(ComponentType type);
		void copy(const ActorCollider* node, ActorArtboard* artboard);
		static ActorCollider* read(ActorArtboard* artboard, BlockReader* reader, ActorCollider* collider);

		bool isCollisionEnabled() const;
		void isCollisionEnabled(bool isIt);
	};

	class ActorColliderRectangle : public ActorCollider
	{
		typedef ActorCollider Base;

	private:
		float m_Width;
		float m_Height;

	public:
		ActorColliderRectangle();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorColliderRectangle* collider, ActorArtboard* artboard);
		static ActorColliderRectangle* read(ActorArtboard* artboard, BlockReader* reader, ActorColliderRectangle* collider = NULL);

		float width() const;
		float height() const;
	};

	class ActorColliderTriangle : public ActorCollider
	{
		typedef ActorCollider Base;

	private:
		float m_Width;
		float m_Height;

	public:
		ActorColliderTriangle();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorColliderTriangle* collider, ActorArtboard* artboard);
		static ActorColliderTriangle* read(ActorArtboard* artboard, BlockReader* reader, ActorColliderTriangle* collider = NULL);

		float width() const;
		float height() const;
	};

	class ActorColliderCircle : public ActorCollider
	{
		typedef ActorCollider Base;

	private:
		float m_Radius;

	public:
		ActorColliderCircle();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorColliderCircle* collider, ActorArtboard* artboard);
		static ActorColliderCircle* read(ActorArtboard* artboard, BlockReader* reader, ActorColliderCircle* collider = NULL);

		float radius() const;
	};

	class ActorColliderPolygon : public ActorCollider
	{
		typedef ActorCollider Base;

	private:
		bool m_IsInstance;
		unsigned int m_ContourVertexCount;
		float* m_ContourVertices;

	public:
		ActorColliderPolygon();
		~ActorColliderPolygon();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorColliderPolygon* collider, ActorArtboard* artboard);
		static ActorColliderPolygon* read(ActorArtboard* artboard, BlockReader* reader, ActorColliderPolygon* collider = NULL);

		float* contourVertices() const;
		unsigned int contourVertexCount() const;
	};

	class ActorColliderLine : public ActorCollider
	{
		typedef ActorCollider Base;

	private:
		bool m_IsInstance;
		unsigned int m_VertexCount;
		float* m_Vertices;

	public:
		ActorColliderLine();
		~ActorColliderLine();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorColliderLine* collider, ActorArtboard* artboard);
		static ActorColliderLine* read(ActorArtboard* artboard, BlockReader* reader, ActorColliderLine* collider = NULL);

		float* vertices() const;
		unsigned int vertexCount() const;
	};
} // namespace flare
#endif