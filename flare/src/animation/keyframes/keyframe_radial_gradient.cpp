#include "flare/animation/keyframes/keyframe_radial_gradient.hpp"
#include "flare/block_reader.hpp"
#include "flare/paint/actor_radial_gradient.hpp"

using namespace flare;

bool KeyFrameRadialGradient::read(BlockReader* reader, ActorComponent* component)
{
	if (dynamic_cast<ActorRadialGradient*>(component) == nullptr || !Base::read(reader, component))
	{
		return false;
	}
	int length = reader->readUint16();
	m_Value.resize(length);
	reader->readFloat32Array(&m_Value[0], length);
	return true;
}

void KeyFrameRadialGradient::apply(ActorComponent* component, float mix)
{
	ActorRadialGradient* gradient = static_cast<ActorRadialGradient*>(component);
	if (gradient == nullptr)
	{
		return;
	}
	int ridx = 0;
	int wi = 0;

	Vec2D start, end;
	int rlength = m_Value.size();
	std::vector<float>& writeColorStops = gradient->m_ColorStops;
	int wlength = writeColorStops.size();

	if (mix == 1.0)
	{
        gradient->secondaryRadiusScale(m_Value[ridx++]);
		start[0] = m_Value[ridx++];
		start[1] = m_Value[ridx++];
		end[0] = m_Value[ridx++];
		end[1] = m_Value[ridx++];

		// use the setters so that markPaintDirty is only called when the start/end values change
		gradient->start(start);
		gradient->end(end);

		while (ridx < rlength && wi < wlength)
		{
			writeColorStops[wi++] = m_Value[ridx++];
		}
	}
	else
	{
		double imix = 1.0 - mix;

        gradient->secondaryRadiusScale(gradient->secondaryRadiusScale() * imix + m_Value[ridx++] * mix);
		start[0] = gradient->m_Start[0] * imix + m_Value[ridx++] * mix;
		start[1] = gradient->m_Start[1] * imix + m_Value[ridx++] * mix;
		end[0] = gradient->m_End[0] * imix + m_Value[ridx++] * mix;
		end[1] = gradient->m_End[1] * imix + m_Value[ridx++] * mix;

		// use the setters so that markPaintDirty is only called when the start/end values change
		gradient->start(start);
		gradient->end(end);

		while (ridx < rlength && wi < wlength)
		{
			writeColorStops[wi] = writeColorStops[wi] * imix + m_Value[ridx++];
			wi++;
		}
	}
	// Invalidate paint only, markPaintDirty will only be called if start/end changed.
	gradient->invalidatePaint();
}

void KeyFrameRadialGradient::applyInterpolation(ActorComponent* component, float time, KeyFrame* toFrame, float mix)
{
	ActorRadialGradient* gradient = static_cast<ActorRadialGradient*>(component);
	if (gradient == nullptr)
	{
		return;
	}
	const std::vector<float>& v = reinterpret_cast<KeyFrameRadialGradient*>(toFrame)->m_Value;
	float f = (time - m_Time) / (toFrame->time() - m_Time);
	if (m_Interpolator != nullptr)
	{
		f = m_Interpolator->getEasedMix(f);
	}
	double fi = 1.0 - f;

	int ridx = 0;
	int wi = 0;
	Vec2D start, end;
	int rlength = m_Value.size();
	std::vector<float>& writeColorStops = gradient->m_ColorStops;
	int wlength = writeColorStops.size();

	if (mix == 1.0f)
	{
        gradient->secondaryRadiusScale(m_Value[ridx] * fi + v[ridx] * f);
        ridx++;
		start[0] = m_Value[ridx] * fi + v[ridx] * f;
		ridx++;
		start[1] = m_Value[ridx] * fi + v[ridx] * f;
		ridx++;
		end[0] = m_Value[ridx] * fi + v[ridx] * f;
		ridx++;
		end[1] = m_Value[ridx] * fi + v[ridx] * f;
		ridx++;

		// use the setters so that markPaintDirty is only called when the start/end values change
		gradient->start(start);
		gradient->end(end);

		while (ridx < rlength && wi < wlength)
		{
			writeColorStops[wi++] = m_Value[ridx] * fi + v[ridx] * f;
			ridx++;
		}
	}
	else
	{
		float imix = 1.0 - mix;

		// Mix : first interpolate the KeyFrames, and then mix on top of the current value.
        float val = m_Value[ridx] * fi + v[ridx] * f;
        gradient->secondaryRadiusScale(m_Value[ridx] * fi + v[ridx] * f);
        ridx++;
        
		val = m_Value[ridx] * fi + v[ridx] * f;
		start[0] = gradient->m_Start[0] * imix + val * mix;
		ridx++;

		val = m_Value[ridx] * fi + v[ridx] * f;
		start[1] = gradient->m_Start[1] * imix + val * mix;
		ridx++;

		val = m_Value[ridx] * fi + v[ridx] * f;
		end[0] = gradient->m_End[0] * imix + val * mix;
		ridx++;

		val = m_Value[ridx] * fi + v[ridx] * f;
		end[1] = gradient->m_End[1] * imix + val * mix;
		ridx++;

		// use the setters so that markPaintDirty is only called when the start/end values change
		gradient->start(start);
		gradient->end(end);

		while (ridx < rlength && wi < wlength)
		{
			val = m_Value[ridx] * fi + v[ridx] * f;
			writeColorStops[wi] = writeColorStops[wi] * imix + val * mix;

			ridx++;
			wi++;
		}
	}
	// Invalidate paint only, markPaintDirty will only be called if start/end changed.
	gradient->invalidatePaint();
}