#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AI���[�h */
	enum class MODE : int
	{
		STANDARD, /* �W�����GAI */
		BERSERK,  /* �o�[�T�[�JAI */
		A_STAR,   /* A-STAR�T�� */
		SIZE
	};
	/* ���E�͈� */
	enum class VISUAL_SIZE : int
	{
		AROUND = 3, /* ���̓}�X(3*3) */

		SMALL  = 6,
		NORMAL = 10,
		LARGE  = 20,
		LLARGE = 500,

		SIZE
	};

	/* �}�b�v�Z�� */
	struct MapCell
	{
		enum class STATUS : int { NONE, OPEN, CLOSE };
		
		int x, y;
		int actual_cost;
		double huristics_cost;
		STATUS status;
		MapCell* parent;

		MapCell() : 
		x(0), y(0),
		actual_cost(0), huristics_cost(0), status(STATUS::NONE),
		parent(nullptr)
		{
		};
		MapCell(const int &px, const int &py, const int& act_cost, const double& hur_cost, MapCell *cell) :
			x(px), y(py),
			actual_cost(act_cost), huristics_cost(hur_cost), status(STATUS::OPEN),
			parent(cell)
		{
		};
		~MapCell()
		{
		};
		inline double GetScoreASTAR()
		{
			return actual_cost + huristics_cost;
		}
	};
};

#endif
