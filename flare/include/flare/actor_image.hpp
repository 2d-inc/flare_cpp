#ifndef _FLARE_ACTORIMAGE_HPP_
#define _FLARE_ACTORIMAGE_HPP_

#include "actor_drawable.hpp"
#include "actor_skinnable.hpp"
#include "blend_mode.hpp"

namespace flare
{
	class Actor;
	class BlockReader;
	class ActorNode;

	struct SequenceFrame
	{
		int m_AtlasIndex;
		int m_Offset;
	};

	class ActorImage : public ActorDrawable, public ActorSkinnable
	{
		typedef ActorDrawable Base;

	private:
		bool m_IsInstance;
		int m_TextureIndex;
		float* m_Vertices;
		unsigned short* m_Triangles;
		int m_VertexCount;
		int m_TriangleCount;
		float* m_AnimationDeformedVertices;

		SequenceFrame* m_SequenceFrames;
		float* m_SequenceUVs;
		int m_SequenceFrame;
		int m_SequenceFramesCount;

	public:
		ActorImage();
		~ActorImage();
		ActorComponent* makeInstance(ActorArtboard* artboard) const override;
		void copy(const ActorImage* node, ActorArtboard* artboard);
		void resolveComponentIndices(ActorComponent** nodes, int numComponents) override;
		bool doesAnimationVertexDeform() const;
		void doesAnimationVertexDeform(bool doesIt);
		float* animationDeformedVertices();
		int textureIndex() const;
		void disposeGeometry();
		int vertexCount() const;
		int triangleCount() const;
		int connectedBoneCount() const;
		int vertexStride() const;

		int boneInfluenceMatricesLength();
		float* boneInfluenceMatrices();

		float* vertices() const;
		unsigned short* triangles() const;
		BlendMode blendMode() const;

		int sequenceFramesCount() const;
		SequenceFrame* sequenceFrames() const;
		float* sequenceUVs() const;
		void sequenceFrame(int value);
		int sequenceFrame() const;

		static ActorImage* read(ActorArtboard* artboard, BlockReader* reader, ActorImage* node = NULL);
		static ActorImage* readImageSequence(ActorArtboard* artboard, BlockReader* reader, ActorImage* node = NULL);
		void invalidateDrawable() override;
		void initializeGraphics() override;
		
		// Help ActorSkinnable find the override call.
		void overrideWorldTransform(const Mat2D& transform) override { ActorNode::overrideWorldTransform(transform); }
	};
} // namespace flare
#endif