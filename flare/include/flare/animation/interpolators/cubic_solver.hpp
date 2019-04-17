#ifndef _FLARE_CUBICSOLVER_HPP_
#define _FLARE_CUBICSOLVER_HPP_

namespace flare
{
	class BlockReader;
	class ActorNode;
	
	class CubicSolver
	{
		private:
			float m_X0;
			float m_Y0;

			float m_X1;
			float m_Y1;

			float m_X2;
			float m_Y2;

			float m_X3;
			float m_Y3;

			float m_E;
			float m_F;
			float m_G;
			float m_H;

		public:
			CubicSolver();
			void set(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
			float get(float x);

	};
}
#endif