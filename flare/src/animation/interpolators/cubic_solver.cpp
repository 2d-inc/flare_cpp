#include "flare/animation/interpolators/cubic_solver.hpp"
#include <cmath>

using namespace flare;
static const float Epsilon = 1e-8f;
CubicSolver::CubicSolver() :
	m_X0(0.0f),
	m_Y0(0.0f),

	m_X1(0.0f),
	m_Y1(0.0f),

	m_X2(0.0f),
	m_Y2(0.0f),

	m_X3(0.0f),
	m_Y3(0.0f),

	m_E(0.0f),
	m_F(0.0f),
	m_G(0.0f),
	m_H(0.0f)
{

}

static float cubicRoot(float v)
{
	if (v < 0.0f)
	{
		return -pow(-v, 1.0f / 3.0f);
	}
	return pow(v, 1.0f / 3.0f);
}

// http://stackoverflow.com/questions/27176423/function-to-solve-cubic-equation-analytically
static int solveCubic(float a, float b, float c, float d, float* roots)
{
	if (fabs(a) < Epsilon)
	{
		// Quadratic case, ax^2+bx+c=0
		a = b;
		b = c;
		c = d;
		if (fabs(a) < Epsilon)
		{
			// Linear case, ax+b=0
			a = b;
			b = c;
			// Degenerate case
			if (fabs(a) < Epsilon)
			{
				return 0;
			}
			else
			{
				roots[0] = -b / a;
				return 1;
			}
		}
		else
		{
			float D = b * b - 4.0 * a * c;
			if (fabs(D) < Epsilon)
			{
				roots[0] = -b / (2.0f * a);
				return 1;
			}
			else if (D > 0.0f)
			{
				roots[0] = (-b + sqrt(D)) / (2.0f * a);
				roots[1] = (-b - sqrt(D)) / (2.0f * a);
				return 2;
			}
		}
		return 0;
	}
	else
	{
		int numRoots = 0;
		// Convert to depressed cubic t^3+pt+q = 0 (subst x = t - b/3a)
		float p = (3.0f * a * c - b * b) / (3.0f * a * a);
		float q = (2.0f * b * b * b - 9.0f * a * b * c + 27.0f * a * a * d) / (27.0f * a * a * a);

		if (fabs(p) < Epsilon)
		{
			// p = 0 -> t^3 = -q -> t = -q^1/3
			roots[0] = cubicRoot(-q);
			numRoots = 0;
		}
		else if (fabs(q) < Epsilon)
		{
			// q = 0 -> t^3 + pt = 0 -> t(t^2+p)=0
			roots[0] = 0.0;
			if (p < 0.0)
			{
				roots[1] = sqrt(-p);
				roots[2] = -sqrt(-p);
				numRoots = 3;
			}
			else
			{
				numRoots = 1;
			}
		}
		else
		{
			float D = q * q / 4.0 + p * p * p / 27.0;
			if (fabs(D) < Epsilon)
			{
				// D = 0 -> two roots
				roots[0] = -1.5 * q / p;
				roots[1] = 3.0 * q / p;
				numRoots = 2;
			}
			else if (D > 0.0)
			{
				// Only one real root
				float u = cubicRoot(-q / 2.0 - sqrt(D));
				roots[0] = u - p / (3.0 * u);
				numRoots = 1;
			}
			else
			{
				// D < 0, three roots, but needs to use complex numbers/trigonometric solution
				float u = 2.0 * sqrt(-p / 3.0);
				float t = acos(3.0 * q / p / u) / 3.0; // D < 0 implies p < 0 and acos argument in [-1..1]
				float k = 2.0 * M_PI / 3.0;
				roots[0] = u * cos(t);
				roots[1] = u * cos(t - k);
				roots[2] = u * cos(t - 2.0 * k);
				numRoots = 3;
			}
		}

		// Convert back from depressed cubic
		for (int i = 0; i < numRoots; i++)
		{
			roots[i] -= b / (3.0 * a);
		}

		return numRoots;
	}
}

void CubicSolver::set(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	m_X0 = x0;
	m_Y0 = y0;

	m_X1 = x1;
	m_Y1 = y1;

	m_X2 = x2;
	m_Y2 = y2;

	m_X3 = x3;
	m_Y3 = y3;

	m_E = y3 - 3.0f * y2 + 3.0f * y1 - y0;
	m_F = 3.0f * y2 - 6.0f * y1 + 3.0f * y0;
	m_G = 3.0f * y1 - 3.0f * y0;
	m_H = y0;
}

float CubicSolver::get(float x)
{
	float p0 = m_X0-x;
	float p1 = m_X1-x;
	float p2 = m_X2-x;
	float p3 = m_X3-x;

	float a = p3 - 3.0 * p2 + 3.0 * p1 - p0;
	float b = 3.0 * p2 - 6.0 * p1 + 3.0 * p0;
	float c = 3.0 * p1 - 3.0 * p0;
	float d = p0;

	float roots[3] = {0.0, 0.0, 0.0};
	float numRoots = solveCubic(a, b, c, d, roots);
	float t = 0.0;
	// Find first valid root.
	for(int i = 0; i < numRoots; i++)
	{
		float r = roots[i];
		if(r >= 0.0 && r <= 1.0)
		{
			t = r;
			break;
		}
	}
	return m_E*(t*t*t) + m_F*(t*t) + m_G*t + m_H;
}