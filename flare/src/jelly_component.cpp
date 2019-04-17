#include "flare/jelly_component.hpp"
#include "flare/actor_bone.hpp"
#include "flare/actor_jelly_bone.hpp"
#include "flare/actor_node.hpp"
#include "flare/block_reader.hpp"

using namespace flare;

const float JellyComponent::OptimalDistance = 4.0f * (std::sqrt(2.0f) - 1.0f) / 3.0f;
const float JellyComponent::CurveConstant = JellyComponent::OptimalDistance * std::sqrt(2.0f) * 0.5f;

JellyComponent::JellyComponent() :
    Base(ComponentType::JellyComponent),
    m_EaseIn(0.0f),
    m_EaseOut(0.0f),
    m_ScaleIn(0.0f),
    m_ScaleOut(0.0f),
    m_InTargetIdx(0),
    m_OutTargetIdx(0),
    m_InTarget(nullptr),
    m_OutTarget(nullptr),
    m_CachedScaleIn(0.0f),
    m_CachedScaleOut(0.0f)
{
	m_NormalizedCurvePoints.reserve(JellyMax);
}

ActorComponent* JellyComponent::makeInstance(ActorArtboard* artboard) const
{
	JellyComponent* instanceNode = new JellyComponent();
	instanceNode->copy(this, artboard);
	return instanceNode;
}

JellyComponent* JellyComponent::read(ActorArtboard* artboard, BlockReader* reader, JellyComponent* node)
{
	if (node == nullptr)
	{
		node = new JellyComponent();
	}

	Base::read(artboard, reader, node);

	node->m_EaseIn = reader->readFloat32();
	node->m_EaseOut = reader->readFloat32();
	node->m_ScaleIn = reader->readFloat32();
	node->m_ScaleOut = reader->readFloat32();
	node->m_InTargetIdx = reader->readUint16();
	node->m_OutTargetIdx = reader->readUint16();

	return node;
}

void JellyComponent::copy(const JellyComponent* node, ActorArtboard* artboard)
{
	Base::copy(node, artboard);
	m_EaseIn = node->m_EaseIn;
	m_EaseOut = node->m_EaseOut;
	m_ScaleIn = node->m_ScaleIn;
	m_ScaleOut = node->m_ScaleOut;
	m_InTargetIdx = node->m_InTargetIdx;
	m_OutTargetIdx = node->m_OutTargetIdx;
}

void JellyComponent::resolveComponentIndices(ActorComponent** components, int numComponents)
{
	Base::resolveComponentIndices(components, numComponents);

	if (m_InTargetIdx != 0 && m_InTargetIdx < numComponents)
	{
		m_InTarget = dynamic_cast<ActorNode*>(components[m_InTargetIdx]);
	}
	if (m_OutTargetIdx != 0 && m_OutTargetIdx < numComponents)
	{
		m_OutTarget = dynamic_cast<ActorNode*>(components[m_OutTargetIdx]);
	}
}

void JellyComponent::completeResolve()
{
	Base::completeResolve();

	ActorBone* bone = dynamic_cast<ActorBone*>(m_Parent);
	assert(bone != nullptr);
	bone->m_Jelly = this;

	// Get jellies.
	const std::vector<ActorNode*>& children = bone->children();
	for (ActorNode* child : children)
	{
		if (child->type() == ComponentType::ActorJellyBone)
		{
			m_Bones.push_back(static_cast<ActorJellyBone*>(child));
		}
	}
}

static const float EPSILON = 0.001f; // Intentionally agressive.

static bool fuzzyEquals(Vec2D a, Vec2D b)
{
	float a0 = a[0], a1 = a[1];
	float b0 = b[0], b1 = b[1];
	return (std::abs(a0 - b0) <= EPSILON * std::max(1.0f, std::max(std::abs(a0), std::abs(b0))) &&
	        std::abs(a1 - b1) <= EPSILON * std::max(1.0f, std::max(std::abs(a1), std::abs(b1))));
}

static void forwardDiffBezier(float c0, float c1, float c2, float c3, Vec2D* points, int count, int offset)
{
	float f = (float)count;

	float p0 = c0;

	float p1 = 3.0f * (c1 - c0) / f;

	f *= count;
	float p2 = 3.0f * (c0 - 2.0f * c1 + c2) / f;

	f *= count;
	float p3 = (c3 - c0 + 3.0f * (c1 - c2)) / f;

	c0 = p0;
	c1 = p1 + p2 + p3;
	c2 = 2 * p2 + 6 * p3;
	c3 = 6 * p3;

	for (int a = 0; a <= count; a++)
	{
		points[a][offset] = c0;
		c0 += c1;
		c1 += c2;
		c2 += c3;
	}
}

// IList<Vec2D> NormalizeCurve(Vec2D[] curve, int numSegments)
// {
//     IList<Vec2D> points = new List<Vec2D>();
//     int curvePointCount = curve.Length;
//     float[] distances = new float[curvePointCount];
//     distances[0] = 0;
//     for(int i = 0; i < curvePointCount-1; i++)
//     {
//         Vec2D p1 = curve[i];
//         Vec2D p2 = curve[i+1];
//         distances[i + 1] = distances[i] + Vec2D.Distance(p1, p2);
//     }
//     float totalDistance = distances[curvePointCount-1];

//     float segmentLength = totalDistance/numSegments;
//     int pointIndex = 1;
//     for(int i = 1; i <= numSegments; i++)
//     {
//         float distance = segmentLength * i;

//         while(pointIndex < curvePointCount-1 && distances[pointIndex] < distance)
//         {
//             pointIndex++;
//         }

//         float d = distances[pointIndex];
//         float lastCurveSegmentLength = d - distances[pointIndex-1];
//         float remainderOfDesired = d - distance;
//         float ratio = remainderOfDesired / lastCurveSegmentLength;
//         float iratio = 1.0f-ratio;

//         Vec2D p1 = curve[pointIndex-1];
//         Vec2D p2 = curve[pointIndex];
//         points.Add(new Vec2D(p1[0]*ratio+p2[0]*iratio, p1[1]*ratio+p2[1]*iratio));
//     }

//     return points;
// }

void normalizeCurve(std::vector<Vec2D>& points, Vec2D* curve, const int numSegments)
{
	constexpr int curvePointCount = JellyComponent::JellyMax + 1;
	float distances[curvePointCount];
	distances[0] = 0;
	for (int i = 0; i < curvePointCount - 1; i++)
	{
		const Vec2D& p1 = curve[i];
		const Vec2D& p2 = curve[i + 1];
		distances[i + 1] = distances[i] + Vec2D::distance(p1, p2);
	}
	float totalDistance = distances[curvePointCount - 1];
	float segmentLength = totalDistance / numSegments;
	int pointIndex = 1;
	points.resize(numSegments);
	for (int i = 1; i <= numSegments; i++)
	{
		float distance = segmentLength * i;

		while (pointIndex < curvePointCount - 1 && distances[pointIndex] < distance)
		{
			pointIndex++;
		}

		float d = distances[pointIndex];
		float lastCurveSegmentLength = d - distances[pointIndex - 1];
		float remainderOfDesired = d - distance;
		float ratio = remainderOfDesired / lastCurveSegmentLength;
		float iratio = 1.0f - ratio;

		const Vec2D& p1 = curve[pointIndex - 1];
		const Vec2D& p2 = curve[pointIndex];

		Vec2D& point = points[i - 1];
		point[0] = p1[0] * ratio + p2[0] * iratio;
		point[1] = p1[1] * ratio + p2[1] * iratio;
	}
}

void JellyComponent::updateJellies()
{
	ActorBone* bone = static_cast<ActorBone*>(m_Parent);
	// We are in local bone space.
	Vec2D tipPosition(bone->length(), 0.0f);

	if (fuzzyEquals(m_CachedTip, tipPosition) && fuzzyEquals(m_CachedOut, m_OutPoint) &&
	    fuzzyEquals(m_CachedIn, m_InPoint) && m_CachedScaleIn == m_ScaleIn && m_CachedScaleOut == m_ScaleOut)
	{
		return;
	}

	Vec2D::copy(m_CachedTip, tipPosition);
	Vec2D::copy(m_CachedOut, m_OutPoint);
	Vec2D::copy(m_CachedIn, m_InPoint);
	m_CachedScaleIn = m_ScaleIn;
	m_CachedScaleOut = m_ScaleOut;

	Vec2D q0;
	Vec2D q1 = m_InPoint;
	Vec2D q2 = m_OutPoint;
	Vec2D q3 = tipPosition;

	forwardDiffBezier(q0[0], q1[0], q2[0], q3[0], m_JellyPoints, JellyMax, 0);
	forwardDiffBezier(q0[1], q1[1], q2[1], q3[1], m_JellyPoints, JellyMax, 1);

	normalizeCurve(m_NormalizedCurvePoints, m_JellyPoints, m_Bones.size());
	// IList<Vec2D> normalizedPoints = normalizeCurve(m_JellyPoints, m_Bones.size());

	Vec2D lastPoint = m_JellyPoints[0];

	float scale = m_ScaleIn;
	float scaleInc = (m_ScaleOut - m_ScaleIn) / (float)(m_Bones.size() - 1);
	for (int i = 0, length = m_NormalizedCurvePoints.size(); i < length; i++)
	{
		ActorJellyBone* jelly = m_Bones[i];
		const Vec2D& p = m_NormalizedCurvePoints[i];

		jelly->translation(lastPoint);
		jelly->length(Vec2D::distance(p, lastPoint));
		jelly->scaleY(scale);
		scale += scaleInc;

		Vec2D diff;
		Vec2D::subtract(diff, p, lastPoint);
		jelly->rotation(std::atan2(diff[1], diff[0]));
		lastPoint = p;
	}
}

void JellyComponent::update(DirtyFlags dirt)
{
	ActorBone* bone = static_cast<ActorBone*>(m_Parent);
	ActorNode* parentBoneNode = bone->parent();
	ActorBone* parentBone;
	if (parentBoneNode != nullptr && parentBoneNode->type() == ComponentType::ActorBone)
	{
		parentBone = static_cast<ActorBone*>(parentBoneNode);
	}
	else
	{
		parentBone = nullptr;
	}

	JellyComponent* parentBoneJelly = parentBone == nullptr ? nullptr : parentBone->m_Jelly;

	Mat2D inverseWorld;
	if (!Mat2D::invert(inverseWorld, bone->worldTransform()))
	{
		return;
	}

	if (m_InTarget != nullptr)
	{
		Vec2D translation;
		m_InTarget->worldTranslation(translation);

		Vec2D::transform(m_InPoint, translation, inverseWorld);
		Vec2D::normalize(m_InDirection, m_InPoint);
	}
	else if (parentBone != nullptr)
	{
		if (parentBone->m_FirstBone == bone && parentBoneJelly != nullptr && parentBoneJelly->m_OutTarget != nullptr)
		{
			Vec2D translation;
			parentBoneJelly->m_OutTarget->worldTranslation(translation);

			Vec2D localParentOut;
			Vec2D::transform(localParentOut, translation, inverseWorld);
			Vec2D::normalize(localParentOut, localParentOut);
			Vec2D::negate(m_InDirection, localParentOut);
		}
		else
		{
			Vec2D d1(1.0f, 0.0f);
			Vec2D d2(1.0f, 0.0f);

			Vec2D::transformDir(d1, d1, parentBone->worldTransform());
			Vec2D::transformDir(d2, d2, bone->worldTransform());

			Vec2D sum;
			Vec2D::add(sum, d1, d2);
			Vec2D::transformDir(m_InDirection, sum, inverseWorld);
			Vec2D::normalize(m_InDirection, m_InDirection);
		}
		m_InPoint[0] = m_InDirection[0] * m_EaseIn * bone->length() * CurveConstant;
		m_InPoint[1] = m_InDirection[1] * m_EaseIn * bone->length() * CurveConstant;
	}
	else
	{
		m_InDirection[0] = 1.0f;
		m_InDirection[1] = 0.0f;
		m_InPoint[0] = m_InDirection[0] * m_EaseIn * bone->length() * CurveConstant;
	}

	if (m_OutTarget != nullptr)
	{
		Vec2D translation;
		m_OutTarget->worldTranslation(translation);
		Vec2D::transform(m_OutPoint, translation, inverseWorld);
		Vec2D tip(bone->length(), 0.0f);
		Vec2D::subtract(m_OutDirection, m_OutPoint, tip);
		Vec2D::normalize(m_OutDirection, m_OutDirection);
	}
	else if (bone->m_FirstBone != nullptr)
	{
		ActorBone* firstBone = bone->m_FirstBone;
		JellyComponent* firstBoneJelly = firstBone->m_Jelly;
		if (firstBoneJelly != nullptr && firstBoneJelly->m_InTarget != nullptr)
		{
			Vec2D translation;
			firstBoneJelly->m_InTarget->worldTranslation(translation);

			Vec2D worldChildInDir;
			firstBone->worldTranslation(worldChildInDir);
			Vec2D::subtract(worldChildInDir, worldChildInDir, translation);
			Vec2D::transformDir(m_OutDirection, worldChildInDir, inverseWorld);
		}
		else
		{
			Vec2D d1(1.0f, 0.0f);
			Vec2D d2(1.0f, 0.0f);

			Vec2D::transformDir(d1, d1, firstBone->worldTransform());
			Vec2D::transformDir(d2, d2, bone->worldTransform());

			Vec2D sum;
			Vec2D::add(sum, d1, d2);
			Vec2D::negate(sum, sum);
			Vec2D::transformDir(m_OutDirection, sum, inverseWorld);
			Vec2D::normalize(m_OutDirection, m_OutDirection);
		}
		Vec2D::normalize(m_OutDirection, m_OutDirection);
		Vec2D scaledOut;
		Vec2D::scale(scaledOut, m_OutDirection, m_EaseOut * bone->length() * CurveConstant);
		m_OutPoint[0] = bone->length();
		m_OutPoint[1] = 0.0f;
		Vec2D::add(m_OutPoint, m_OutPoint, scaledOut);
	}
	else
	{
		m_OutDirection[0] = -1.0f;
		m_OutDirection[1] = 0.0f;

		Vec2D scaledOut;
		Vec2D::scale(scaledOut, m_OutDirection, m_EaseOut * bone->length() * CurveConstant);
		m_OutPoint[0] = bone->length();
		m_OutPoint[1] = 0.0f;
		Vec2D::add(m_OutPoint, m_OutPoint, scaledOut);
	}

	updateJellies();
}