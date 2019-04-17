#include "flare/animation/property_animation.hpp"
#include "flare/animation/keyframes/keyframe.hpp"
#include "flare/animation/keyframes/keyframe_active_child.hpp"
#include "flare/animation/keyframes/keyframe_color.hpp"
#include "flare/animation/keyframes/keyframe_constraint_strength.hpp"
#include "flare/animation/keyframes/keyframe_corner_radius.hpp"
#include "flare/animation/keyframes/keyframe_inner_radius.hpp"
#include "flare/animation/keyframes/keyframe_stroke_start.hpp"
#include "flare/animation/keyframes/keyframe_stroke_end.hpp"
#include "flare/animation/keyframes/keyframe_stroke_offset.hpp"
#include "flare/animation/keyframes/keyframe_custom_property.hpp"
#include "flare/animation/keyframes/keyframe_draw_order.hpp"
#include "flare/animation/keyframes/keyframe_gradient.hpp"
#include "flare/animation/keyframes/keyframe_image_vertices.hpp"
#include "flare/animation/keyframes/keyframe_is_collision_enabled.hpp"
#include "flare/animation/keyframes/keyframe_length.hpp"
#include "flare/animation/keyframes/keyframe_opacity.hpp"
#include "flare/animation/keyframes/keyframe_paint_opacity.hpp"
#include "flare/animation/keyframes/keyframe_path_vertices.hpp"
#include "flare/animation/keyframes/keyframe_pos_x.hpp"
#include "flare/animation/keyframes/keyframe_pos_y.hpp"
#include "flare/animation/keyframes/keyframe_radial_gradient.hpp"
#include "flare/animation/keyframes/keyframe_rotation.hpp"
#include "flare/animation/keyframes/keyframe_scale_x.hpp"
#include "flare/animation/keyframes/keyframe_scale_y.hpp"
#include "flare/animation/keyframes/keyframe_sequence.hpp"
#include "flare/animation/keyframes/keyframe_shape_height.hpp"
#include "flare/animation/keyframes/keyframe_shape_width.hpp"
#include "flare/animation/keyframes/keyframe_stroke_width.hpp"
#include "flare/animation/keyframes/keyframe_trigger.hpp"
#include "flare/block_reader.hpp"
#include <cassert>

using namespace flare;

PropertyAnimation::PropertyAnimation() : m_Type(PropertyType::Unknown) {}

PropertyAnimation::~PropertyAnimation()
{
	for (auto keyFrame : m_KeyFrames)
	{
		delete keyFrame;
	}
}

int PropertyAnimation::keyFramesCount() const { return m_KeyFrames.size(); }

PropertyType PropertyAnimation::type() const { return m_Type; }

void PropertyAnimation::read(BlockReader* reader, ActorComponent* component)
{
	BlockReader* block = reader->readNextBlock();
	if (block == nullptr)
	{
		return;
	}
	m_Type = block->blockType<PropertyType>();
	if (m_Type >= PropertyType::Max)
	{
		block->close();
		return;
	}
	int keyFrameCount = (int)block->readUint16();
	KeyFrame* lastKeyFrame = nullptr;
	for (int i = 0; i < keyFrameCount; i++)
	{
		KeyFrame* frame = nullptr;
		switch (m_Type)
		{
			case PropertyType::PosX:
				frame = new KeyFramePosX();
				break;
			case PropertyType::PosY:
				frame = new KeyFramePosY();
				break;
			case PropertyType::ScaleX:
				frame = new KeyFrameScaleX();
				break;
			case PropertyType::ScaleY:
				frame = new KeyFrameScaleY();
				break;
			case PropertyType::Rotation:
				frame = new KeyFrameRotation();
				break;
			case PropertyType::Opacity:
				frame = new KeyFrameOpacity();
				break;
			case PropertyType::DrawOrder:
				frame = new KeyFrameDrawOrder();
				break;
			case PropertyType::Length:
				frame = new KeyFrameLength();
				break;
			case PropertyType::ImageVertices:
				frame = new KeyFrameImageVertices();
				break;
			case PropertyType::ConstraintStrength:
				frame = new KeyFrameConstraintStrength();
				break;
			case PropertyType::Trigger:
				frame = new KeyFrameTrigger();
				break;
			case PropertyType::IntProperty:
				frame = new KeyFrameIntProperty();
				break;
			case PropertyType::FloatProperty:
				frame = new KeyFrameFloatProperty();
				break;
			case PropertyType::StringProperty:
				frame = new KeyFrameStringProperty();
				break;
			case PropertyType::BooleanProperty:
				frame = new KeyFrameBooleanProperty();
				break;
			case PropertyType::IsCollisionEnabled:
				frame = new KeyFrameIsCollisionEnabled();
				break;
			case PropertyType::ActiveChildIndex:
				frame = new KeyFrameActiveChild();
				break;
			case PropertyType::Sequence:
				frame = new KeyFrameSequence();
				break;
			case PropertyType::PathVertices:
				frame = new KeyFramePathVertices();
				break;
			case PropertyType::FillColor:
			case PropertyType::StrokeColor:
				frame = new KeyFrameColor();
				break;
			case PropertyType::FillGradient:
			case PropertyType::StrokeGradient:
				frame = new KeyFrameGradient();
				break;
			case PropertyType::FillRadial:
			case PropertyType::StrokeRadial:
				frame = new KeyFrameRadialGradient();
				break;
			case PropertyType::StrokeWidth:
				frame = new KeyFrameStrokeWidth();
				break;
			case PropertyType::StrokeOpacity:
			case PropertyType::FillOpacity:
				frame = new KeyFramePaintOpacity();
				break;
			case PropertyType::ShapeWidth:
				frame = new KeyFrameShapeWidth();
				break;
			case PropertyType::ShapeHeight:
				frame = new KeyFrameShapeHeight();
				break;
			case PropertyType::CornerRadius:
				frame = new KeyFrameCornerRadius();
				break;
			case PropertyType::InnerRadius:
				frame = new KeyFrameInnerRadius();
				break;
			case PropertyType::StrokeStart:
				frame = new KeyFrameStrokeStart();
				break;
			default:
				// This will only happen if the code isn't handling a property type it should handle.
				// Check the PropertyType enum and make sure Max is in the right place (and that you're not missing a
				// case).
				assert(false);
				break;
		}

		if (frame->read(block, component))
		{
			m_KeyFrames.emplace_back(frame);
			if (lastKeyFrame != nullptr)
			{
				lastKeyFrame->setNext(frame);
			}
			lastKeyFrame = frame;
		}
		else
		{
			// If the frame fails to load, we just remove it from the list, not we change our list length (no we don't
			// realloc the list length).
			delete frame;
		}
	}
	block->close();
}

void PropertyAnimation::apply(float time, ActorComponent* component, float mix)
{
	if (m_KeyFrames.empty())
	{
		return;
	}

	int idx = 0;
	// Binary find the keyframe index.
	{
		int mid = 0;
		float element = 0.0f;
		int start = 0;
		int end = m_KeyFrames.size() - 1;

		while (start <= end)
		{
			mid = ((start + end) >> 1);
			element = m_KeyFrames[mid]->time();
			if (element < time)
			{
				start = mid + 1;
			}
			else if (element > time)
			{
				end = mid - 1;
			}
			else
			{
				start = mid;
				break;
			}
		}

		idx = start;
	}

	if (idx == 0)
	{
		m_KeyFrames[0]->apply(component, mix);
	}
	else
	{
		if (idx < m_KeyFrames.size())
		{
			KeyFrame* fromFrame = m_KeyFrames[idx - 1];
			KeyFrame* toFrame = m_KeyFrames[idx];
			if (time == toFrame->time())
			{
				toFrame->apply(component, mix);
			}
			else
			{
				fromFrame->applyInterpolation(component, time, toFrame, mix);
			}
		}
		else
		{
			m_KeyFrames[idx - 1]->apply(component, mix);
		}
	}
}