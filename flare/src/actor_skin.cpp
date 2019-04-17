#include "flare/actor_skin.hpp"
#include "flare/actor_artboard.hpp"
#include "flare/actor_skinnable.hpp"
#include "flare_math/mat2d.hpp"

using namespace flare;

ActorSkin::ActorSkin() :
    ActorComponent(ComponentType::ActorSkin),
    m_BoneMatrices(nullptr),
    m_BoneMatricesLength(0),
    m_SkinnableParent(nullptr)
{
}

ActorSkin::~ActorSkin() { delete[] m_BoneMatrices; }

const float* ActorSkin::boneMatrices() const { return m_BoneMatrices; }

void ActorSkin::onParentChanged(ActorNode* from, ActorNode* to)
{
	m_SkinnableParent = dynamic_cast<ActorSkinnable*>(to);
}

void ActorSkin::update(DirtyFlags dirt)
{
	if (m_SkinnableParent == nullptr || !m_SkinnableParent->isConnectedToBones())
	{
		return;
	}

	auto& connectedBones = m_SkinnableParent->m_ConnectedBones;

	int length = (connectedBones.size() + 1) * 6;
	if (m_BoneMatricesLength != length)
	{
		delete[] m_BoneMatrices;
		m_BoneMatricesLength = length;
		m_BoneMatrices = new float[length];
		// First bone transform is always identity.
		m_BoneMatrices[0] = 1.0;
		m_BoneMatrices[1] = 0.0;
		m_BoneMatrices[2] = 0.0;
		m_BoneMatrices[3] = 1.0;
		m_BoneMatrices[4] = 0.0;
		m_BoneMatrices[5] = 0.0;
	}

	int bidx = 6; // Start after first identity.

	Mat2D mat;

	for (const auto& skinnedBone : connectedBones)
	{
		if (skinnedBone.node == nullptr)
		{
			m_BoneMatrices[bidx++] = 1.0;
			m_BoneMatrices[bidx++] = 0.0;
			m_BoneMatrices[bidx++] = 0.0;
			m_BoneMatrices[bidx++] = 1.0;
			m_BoneMatrices[bidx++] = 0.0;
			m_BoneMatrices[bidx++] = 0.0;
			continue;
		}

		Mat2D::multiply(mat, skinnedBone.node->worldTransform(), skinnedBone.inverseBind);

		m_BoneMatrices[bidx++] = mat[0];
		m_BoneMatrices[bidx++] = mat[1];
		m_BoneMatrices[bidx++] = mat[2];
		m_BoneMatrices[bidx++] = mat[3];
		m_BoneMatrices[bidx++] = mat[4];
		m_BoneMatrices[bidx++] = mat[5];
	}

	m_SkinnableParent->invalidateDrawable();
}

void ActorSkin::completeResolve()
{
	Base::completeResolve();

	// parent will have resolved already.
	if (m_SkinnableParent == nullptr)
	{
		return;
	}
	m_SkinnableParent->m_Skin = this;
	// This shouldn't be necessary as ActorComponent set parent dependency already.
	// m_Artboard->addDependency(this, m_SkinnableParent);
	if (m_SkinnableParent->isConnectedToBones())
	{
		auto& connectedBones = m_SkinnableParent->m_ConnectedBones;
		for (const auto& skinnedBone : connectedBones)
		{
			m_Artboard->addDependency(this, skinnedBone.node);

			std::vector<ActorConstraint*> allConstraints = skinnedBone.node->allConstraints();
			for (auto constraint : allConstraints)
			{
				m_Artboard->addDependency(this, constraint);
			}
		}
	}
}

ActorComponent* ActorSkin::makeInstance(ActorArtboard* artboard) const
{
	ActorSkin* instance = new ActorSkin();
	instance->copy(this, artboard);
	return instance;
}